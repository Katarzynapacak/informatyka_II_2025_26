"""
model.py
Logika procesu + obiekty (zbiorniki, rury, pompa, grzałka) + alarmy.
Minimalnie, ale czytelnie - w stylu 2 rok automatyki.
"""

from __future__ import annotations
from dataclasses import dataclass
from typing import Callable, List, Tuple
import math

from PyQt5.QtCore import Qt, QPointF
from PyQt5.QtGui import QPainter, QColor, QPen, QPainterPath


# -------------------------
# Sygnały (jak w automatyce)
# -------------------------

class Sygnal:
    def __init__(self, wartosc):
        self.wartosc = wartosc


class SygnalFloat(Sygnal):
    wartosc: float


class SygnalBool(Sygnal):
    wartosc: bool


# -------------------------
# Alarm
# -------------------------

@dataclass
class Alarm:
    tag: str
    opis: str
    warunek: Callable[[], bool]
    aktywny: bool = False

    def aktualizuj(self) -> None:
        self.aktywny = bool(self.warunek())


# -------------------------
# Elementy procesu
# -------------------------

class Zbiornik:
    def __init__(self, x: int, y: int, w: int, h: int, nazwa: str):
        self.x, self.y, self.w, self.h = x, y, w, h
        self.nazwa = nazwa

        self.pojemnosc = 100.0
        self.ilosc = 0.0

        self.poziom = SygnalFloat(0.0)        # 0..1
        self.temperatura = SygnalFloat(20.0)  # °C

        self._kolor_cieczy = QColor(0, 140, 255, 190)

    # --- wygodne metody do bilansu masy ---
    def ustaw_ilosc(self, ilosc: float) -> None:
        self.ilosc = max(0.0, min(self.pojemnosc, float(ilosc)))
        self._aktualizuj_poziom()

    def dodaj(self, ilosc: float) -> float:
        wolne = self.pojemnosc - self.ilosc
        dodano = min(float(ilosc), wolne)
        self.ilosc += dodano
        self._aktualizuj_poziom()
        return dodano

    def zabierz(self, ilosc: float) -> float:
        zabrano = min(float(ilosc), self.ilosc)
        self.ilosc -= zabrano
        self._aktualizuj_poziom()
        return zabrano

    def czy_pusty(self) -> bool:
        return self.ilosc <= 0.1

    def _aktualizuj_poziom(self) -> None:
        self.poziom.wartosc = self.ilosc / self.pojemnosc if self.pojemnosc > 0 else 0.0

    # --- punkty zaczepienia rur ---
    def punkt_gora(self) -> Tuple[float, float]:
        return (self.x + self.w / 2, self.y)

    def punkt_dol(self) -> Tuple[float, float]:
        return (self.x + self.w / 2, self.y + self.h)

    # --- rysowanie ---
    def rysuj(self, p: QPainter) -> None:
        # ciecz
        if self.poziom.wartosc > 0:
            hh = self.h * self.poziom.wartosc
            y0 = self.y + self.h - hh
            p.setPen(Qt.NoPen)
            p.setBrush(self._kolor_cieczy)
            p.drawRect(int(self.x + 3), int(y0 + 2), int(self.w - 6), int(hh - 4))

        # obrys
        pen = QPen(Qt.white, 3)
        p.setPen(pen)
        p.setBrush(Qt.NoBrush)
        p.drawRect(self.x, self.y, self.w, self.h)

        # opisy
        p.setPen(Qt.white)
        p.drawText(self.x, self.y - 8, f"{self.nazwa}")
        p.drawText(self.x, self.y + self.h + 16, f"{self.poziom.wartosc*100:5.1f}%")
        p.drawText(self.x, self.y + self.h + 32, f"T={self.temperatura.wartosc:4.1f}°C")


class Rura:
    def __init__(self, punkty: List[Tuple[float, float]], grubosc: int = 12):
        self.punkty = [QPointF(float(x), float(y)) for x, y in punkty]
        self.grubosc = grubosc
        self.kolor_rury = QColor(140, 140, 140)
        self.kolor_cieczy = QColor(0, 180, 255)
        self.czy_plynie = False

    def ustaw_przeplyw(self, plynie: bool) -> None:
        self.czy_plynie = bool(plynie)

    def rysuj(self, p: QPainter) -> None:
        if len(self.punkty) < 2:
            return

        sciezka = QPainterPath()
        sciezka.moveTo(self.punkty[0])
        for pt in self.punkty[1:]:
            sciezka.lineTo(pt)

        # obudowa
        p.setBrush(Qt.NoBrush)
        p.setPen(QPen(self.kolor_rury, self.grubosc, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
        p.drawPath(sciezka)

        # "ciecz"
        if self.czy_plynie:
            p.setPen(QPen(self.kolor_cieczy, self.grubosc - 4, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
            p.drawPath(sciezka)


class Pompa:
    def __init__(self, x: int, y: int, nazwa: str):
        self.x, self.y = x, y
        self.nazwa = nazwa
        self.wlaczona = SygnalBool(True)
        self.predkosc = SygnalFloat(1.0)
        self._kat = 0.0

    def krok_animacji(self, dt: float) -> None:
        if self.wlaczona.wartosc:
            self._kat += (8.0 * self.predkosc.wartosc) * dt

    def rysuj(self, p: QPainter) -> None:
        r = 22
        cx, cy = self.x, self.y

        p.setPen(QPen(Qt.white, 2))
        p.setBrush(QColor(50, 50, 50))
        p.drawEllipse(cx - r, cy - r, 2 * r, 2 * r)

        # wirnik
        if self.wlaczona.wartosc:
            p.setPen(QPen(QColor(0, 220, 255), 2))
            for i in range(4):
                a = self._kat + i * (math.pi / 2)
                x2 = cx + 16 * math.cos(a)
                y2 = cy + 16 * math.sin(a)
                p.drawLine(QPointF(cx, cy), QPointF(x2, y2))
        else:
            p.setPen(QPen(QColor(255, 120, 120), 2))
            p.drawLine(cx - 10, cy - 10, cx + 10, cy + 10)

        p.setPen(Qt.white)
        p.drawText(cx - 18, cy + 38, self.nazwa)


class Grzalka:
    def __init__(self, x: int, y: int, nazwa: str):
        self.x, self.y = x, y
        self.nazwa = nazwa
        self.wlaczona = SygnalBool(True)
        self.moc = SygnalFloat(3.0)

    def rysuj(self, p: QPainter) -> None:
        x, y = self.x, self.y
        w, h = 60, 18

        p.setPen(QPen(Qt.white, 2))
        if self.wlaczona.wartosc:
            p.setBrush(QColor(255, 140, 0))
        else:
            p.setBrush(QColor(80, 80, 80))

        p.drawRoundedRect(x, y, w, h, 6, 6)
        p.setPen(Qt.white)
        p.drawText(x, y - 6, f"{self.nazwa}  {self.moc.wartosc:.1f}")


# -------------------------
# Model procesu
# -------------------------

class ModelInstalacji:
    def __init__(self):
        # elementy
        self.z1 = Zbiornik(60, 70, 110, 150, "Z1")
        self.z2 = Zbiornik(270, 70, 110, 150, "Z2")
        self.z3 = Zbiornik(480, 70, 110, 150, "Z3")
        self.z4 = Zbiornik(270, 320, 110, 150, "Z4")
        self.zbiorniki = [self.z1, self.z2, self.z3, self.z4]

        self.pompa = Pompa(200, 300, "P1")
        self.grzalka = Grzalka(535, 255, "H1")

        self.wezel_T = (400, 250)

        self.bazowy_przeplyw = 18.0
        self.temp_otoczenia = 20.0

        # rury (90° + rozgałęzienie)
        self.rura_z1_do_pompy = Rura(self.sciezka_L(self.z1.punkt_dol(), (self.pompa.x - 25, self.pompa.y)))
        self.rura_pompa_do_T = Rura(self.sciezka_L((self.pompa.x + 25, self.pompa.y), self.wezel_T))
        self.rura_T_do_z2 = Rura(self.sciezka_L(self.wezel_T, self.z2.punkt_dol()))
        self.rura_T_do_z3 = Rura(self.sciezka_L(self.wezel_T, self.z3.punkt_dol()))
        self.rura_z2_do_z4 = Rura(self.sciezka_L(self.z2.punkt_dol(), self.z4.punkt_gora()))
        self.rura_z3_do_z4 = Rura(self.sciezka_L(self.z3.punkt_dol(), self.z4.punkt_gora()))
        self.rura_z4_do_z1 = Rura(self.sciezka_L(self.z4.punkt_dol(), self.z1.punkt_gora()))

        self.rury = [
            self.rura_z1_do_pompy, self.rura_pompa_do_T,
            self.rura_T_do_z2, self.rura_T_do_z3,
            self.rura_z2_do_z4, self.rura_z3_do_z4,
            self.rura_z4_do_z1
        ]

        # alarmy
        self.alarmy: List[Alarm] = [
            Alarm("Z2.HH", "Przepełnienie Z2 (>95%)", lambda: self.z2.poziom.wartosc > 0.95),
            Alarm("Z3.HH", "Przepełnienie Z3 (>95%)", lambda: self.z3.poziom.wartosc > 0.95),
            Alarm("Z4.HH", "Przepełnienie Z4 (>95%)", lambda: self.z4.poziom.wartosc > 0.95),
            Alarm("T3.HI", "Wysoka temperatura Z3 (>80°C)", lambda: self.z3.temperatura.wartosc > 80.0),
            Alarm("P1.TRIP", "Pompa wyłączona przy zapotrzebowaniu",
                  lambda: (not self.pompa.wlaczona.wartosc) and (self.z1.poziom.wartosc > 0.2)),
        ]

        # trendy
        self.t = 0.0
        self._akum_probki = 0.0
        self.historia = []  # (t, z1, z2, z3, z4, tempZ3)

        # anty-miganie rur
        self._hold = {name: 0.0 for name in ["z1p", "pT", "Tz2", "Tz3", "z2z4", "z3z4", "z4z1"]}

    @staticmethod
    def sciezka_L(p1: Tuple[float, float], p2: Tuple[float, float]) -> List[Tuple[float, float]]:
        x1, y1 = p1
        x2, y2 = p2
        mid_y = (y1 + y2) / 2
        return [(x1, y1), (x1, mid_y), (x2, mid_y), (x2, y2)]

    def ustaw_parametry_startowe(
        self,
        z1_proc: float, z2_proc: float, z3_proc: float, z4_proc: float,
        predkosc_pompy: float, moc_grzalki: float,
        pompa_on: bool, grzalka_on: bool,
        temp_start: float
    ) -> None:
        self.z1.ustaw_ilosc(self.z1.pojemnosc * z1_proc / 100.0)
        self.z2.ustaw_ilosc(self.z2.pojemnosc * z2_proc / 100.0)
        self.z3.ustaw_ilosc(self.z3.pojemnosc * z3_proc / 100.0)
        self.z4.ustaw_ilosc(self.z4.pojemnosc * z4_proc / 100.0)

        for z in self.zbiorniki:
            z.temperatura.wartosc = float(temp_start)

        self.pompa.predkosc.wartosc = float(predkosc_pompy)
        self.pompa.wlaczona.wartosc = bool(pompa_on)

        self.grzalka.moc.wartosc = float(moc_grzalki)
        self.grzalka.wlaczona.wartosc = bool(grzalka_on)

        self.t = 0.0
        self._akum_probki = 0.0
        self.historia.clear()
        for k in self._hold:
            self._hold[k] = 0.0

    def krok(self, dt: float) -> None:
        self.t += dt
        self.pompa.krok_animacji(dt)

        # przepływ z pompy
        przeplyw = 0.0
        if self.pompa.wlaczona.wartosc and (not self.z1.czy_pusty()):
            przeplyw = self.bazowy_przeplyw * self.pompa.predkosc.wartosc

        # Z1 -> pompa -> T
        ile_z_z1 = self.z1.zabierz(przeplyw * dt) if przeplyw > 0 else 0.0

        # rozdział w T na Z2 i Z3 wg wolnego miejsca
        wolne_z2 = max(0.0, self.z2.pojemnosc - self.z2.ilosc)
        wolne_z3 = max(0.0, self.z3.pojemnosc - self.z3.ilosc)
        suma = wolne_z2 + wolne_z3

        do_z2 = do_z3 = 0.0
        if ile_z_z1 > 0 and suma > 0:
            udzial_z2 = wolne_z2 / suma
            do_z2 = self.z2.dodaj(ile_z_z1 * udzial_z2)
            do_z3 = self.z3.dodaj(ile_z_z1 - do_z2)

        # spływ do Z4
        graw = 7.0 * dt
        z2_do_z4 = self.z2.zabierz(graw) if self.z2.ilosc > 2.0 else 0.0
        z3_do_z4 = self.z3.zabierz(graw) if self.z3.ilosc > 2.0 else 0.0
        self.z4.dodaj(z2_do_z4 + z3_do_z4)

        # powrót zależny od poziomu Z4 (stabilizacja bilansu)
        powrot_na_s = 8.0 + 20.0 * self.z4.poziom.wartosc
        z4_do_z1 = self.z4.zabierz(powrot_na_s * dt) if self.z4.ilosc > 1.0 else 0.0
        self.z1.dodaj(z4_do_z1)

        # przelew awaryjny (żeby nie dobijać do 100%)
        if self.z4.poziom.wartosc > 0.97:
            self.z4.zabierz(25.0 * dt)

        # grzanie Z3 + chłodzenie do otoczenia
        if self.grzalka.wlaczona.wartosc and self.z3.poziom.wartosc > 0.10:
            self.z3.temperatura.wartosc += (2.5 * self.grzalka.moc.wartosc) * dt

        for z in self.zbiorniki:
            z.temperatura.wartosc += (self.temp_otoczenia - z.temperatura.wartosc) * (0.08 * dt)

        # anty-miganie rur (podtrzymanie)
        HOLD = 0.30
        PROG = 0.05

        def podtrzymaj(key: str, warunek: bool) -> bool:
            if warunek:
                self._hold[key] = HOLD
            else:
                self._hold[key] = max(0.0, self._hold[key] - dt)
            return self._hold[key] > 0.0

        plynie_z1p = podtrzymaj("z1p", ile_z_z1 > PROG)
        plynie_pT = podtrzymaj("pT", ile_z_z1 > PROG)
        plynie_Tz2 = podtrzymaj("Tz2", do_z2 > PROG)
        plynie_Tz3 = podtrzymaj("Tz3", do_z3 > PROG)
        plynie_z2z4 = podtrzymaj("z2z4", z2_do_z4 > PROG)
        plynie_z3z4 = podtrzymaj("z3z4", z3_do_z4 > PROG)
        plynie_z4z1 = podtrzymaj("z4z1", z4_do_z1 > PROG)

        self.rura_z1_do_pompy.ustaw_przeplyw(plynie_z1p)
        self.rura_pompa_do_T.ustaw_przeplyw(plynie_pT)
        self.rura_T_do_z2.ustaw_przeplyw(plynie_Tz2)
        self.rura_T_do_z3.ustaw_przeplyw(plynie_Tz3)
        self.rura_z2_do_z4.ustaw_przeplyw(plynie_z2z4)
        self.rura_z3_do_z4.ustaw_przeplyw(plynie_z3z4)
        self.rura_z4_do_z1.ustaw_przeplyw(plynie_z4z1)

        # alarmy
        for a in self.alarmy:
            a.aktualizuj()

        # trendy (co 0.2 s)
        self._akum_probki += dt
        if self._akum_probki >= 0.2:
            self._akum_probki = 0.0
            self.historia.append((
                self.t,
                self.z1.poziom.wartosc,
                self.z2.poziom.wartosc,
                self.z3.poziom.wartosc,
                self.z4.poziom.wartosc,
                self.z3.temperatura.wartosc
            ))
            if len(self.historia) > 600:
                self.historia = self.historia[-600:]
