import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton
from PyQt5.QtCore import Qt, QTimer, QPointF
from PyQt5.QtGui import QPainter, QColor, QPen, QPainterPath


class Rura:
    def __init__(self, punkty, grubosc=12, kolor=Qt.gray):
        # Konwersja listy krotek na QPointF
        self.punkty = [QPointF(float(p[0]), float(p[1])) for p in punkty]
        self.grubosc = grubosc
        self.kolor_rury = kolor
        self.kolor_cieczy = QColor(0, 180, 255)  # jasny niebieski
        self.czy_plynie = False

    def ustaw_przeplyw(self, plynie: bool):
        self.czy_plynie = plynie

    def draw(self, painter: QPainter):
        if len(self.punkty) < 2:
            return

        path = QPainterPath()
        path.moveTo(self.punkty[0])
        for p in self.punkty[1:]:
            path.lineTo(p)

        # 1) Obudowa rury
        pen_rura = QPen(self.kolor_rury, self.grubosc, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin)
        painter.setPen(pen_rura)
        painter.setBrush(Qt.NoBrush)
        painter.drawPath(path)

        # 2) Ciecz w srodku (gdy plynie)
        if self.czy_plynie:
            pen_ciecz = QPen(self.kolor_cieczy, self.grubosc - 4, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin)
            painter.setPen(pen_ciecz)
            painter.drawPath(path)


class Zbiornik:
    def __init__(self, x, y, width=100, height=140, nazwa=""):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.nazwa = nazwa

        self.pojemnosc = 100.0
        self.aktualna_ilosc = 0.0
        self.poziom = 0.0  # 0..1

    def dodaj_ciecz(self, ilosc: float) -> float:
        wolne = self.pojemnosc - self.aktualna_ilosc
        dodano = min(ilosc, wolne)
        self.aktualna_ilosc += dodano
        self.aktualizuj_poziom()
        return dodano

    def usun_ciecz(self, ilosc: float) -> float:
        usunieto = min(ilosc, self.aktualna_ilosc)
        self.aktualna_ilosc -= usunieto
        self.aktualizuj_poziom()
        return usunieto

    def aktualizuj_poziom(self):
        self.poziom = self.aktualna_ilosc / self.pojemnosc if self.pojemnosc > 0 else 0.0

    def czy_pusty(self) -> bool:
        return self.aktualna_ilosc <= 0.1

    def czy_pelny(self) -> bool:
        return self.aktualna_ilosc >= self.pojemnosc - 0.1

    # Punkty zaczepienia rur
    def punkt_gora_srodek(self):
        return (self.x + self.width / 2, self.y)

    def punkt_dol_srodek(self):
        return (self.x + self.width / 2, self.y + self.height)

    def draw(self, painter: QPainter):
        # 1) Ciecz
        if self.poziom > 0:
            h_cieczy = self.height * self.poziom
            y_start = self.y + self.height - h_cieczy
            painter.setPen(Qt.NoPen)
            painter.setBrush(QColor(0, 120, 255, 200))
            painter.drawRect(int(self.x + 3), int(y_start), int(self.width - 6), int(h_cieczy - 2))

        # 2) Obrys
        pen = QPen(Qt.white, 4)
        pen.setJoinStyle(Qt.MiterJoin)
        painter.setPen(pen)
        painter.setBrush(Qt.NoBrush)
        painter.drawRect(int(self.x), int(self.y), int(self.width), int(self.height))

        # 3) Podpis
        painter.setPen(Qt.white)
        painter.drawText(int(self.x), int(self.y - 10), self.nazwa)


class SymulacjaKaskady(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Kaskada: Dol -> Gora")
        self.setFixedSize(900, 600)
        self.setStyleSheet("background-color: #222;")

        # --- Zbiorniki (schodkowo) ---
        self.z1 = Zbiornik(50, 50, nazwa="Zbiornik 1")
        self.z1.aktualna_ilosc = 100.0
        self.z1.aktualizuj_poziom()

        self.z2 = Zbiornik(350, 200, nazwa="Zbiornik 2")
        self.z3 = Zbiornik(650, 350, nazwa="Zbiornik 3")

        self.zbiorniki = [self.z1, self.z2, self.z3]

        # --- Rury ---
        # Rura 1: Z1 (dol) -> Z2 (gora)
        p_start = self.z1.punkt_dol_srodek()
        p_koniec = self.z2.punkt_gora_srodek()
        mid_y = (p_start[1] + p_koniec[1]) / 2

        self.rura1 = Rura([
            p_start,
            (p_start[0], mid_y),
            (p_koniec[0], mid_y),
            p_koniec
        ])

        # Rura 2: Z2 (dol) -> Z3 (gora)
        p_start2 = self.z2.punkt_dol_srodek()
        p_koniec2 = self.z3.punkt_gora_srodek()
        mid_y2 = (p_start2[1] + p_koniec2[1]) / 2

        self.rura2 = Rura([
            p_start2,
            (p_start2[0], mid_y2),
            (p_koniec2[0], mid_y2),
            p_koniec2
        ])

        self.rury = [self.rura1, self.rura2]

        # --- Timer i parametry przeplywu ---
        self.timer = QTimer()
        self.timer.timeout.connect(self.logika_przeplywu)

        self.running = False
        self.flow_speed = 0.8

        # --- UI: Start/Stop + panel przyciskow dla Z1/Z2/Z3 ---
        self._zbuduj_panel_sterowania()

    def _zbuduj_panel_sterowania(self):
        # Start/Stop
        self.btn_start = QPushButton("Start / Stop", self)
        self.btn_start.setGeometry(50, 550, 120, 30)
        self.btn_start.setStyleSheet("background-color: #444; color: white;")
        self.btn_start.clicked.connect(self.przelacz_symulacje)

        # Panel dla zbiornikow
        # (uklad: Z1 [+][-]  Z2 [+][-]  Z3 [+][-])
        x0, y0 = 200, 550
        dx = 210

        def mk_btn(text, x, cb):
            b = QPushButton(text, self)
            b.setGeometry(x, y0, 45, 30)
            b.setStyleSheet("background-color: #444; color: white;")
            b.clicked.connect(cb)
            return b

        def mk_label(text, x):
            b = QPushButton(text, self)
            b.setGeometry(x, y0, 90, 30)
            b.setEnabled(False)
            b.setStyleSheet("background-color: #333; color: white; border: 1px solid #555;")
            return b

        # Z1
        mk_label("Z1", x0, )
        self.btn_z1_plus = mk_btn("+", x0 + 95, lambda: self.napelnij(self.z1))
        self.btn_z1_minus = mk_btn("-", x0 + 145, lambda: self.oproznij(self.z1))

        # Z2
        mk_label("Z2", x0 + dx, )
        self.btn_z2_plus = mk_btn("+", x0 + dx + 95, lambda: self.napelnij(self.z2))
        self.btn_z2_minus = mk_btn("-", x0 + dx + 145, lambda: self.oproznij(self.z2))

        # Z3
        mk_label("Z3", x0 + 2 * dx, )
        self.btn_z3_plus = mk_btn("+", x0 + 2 * dx + 95, lambda: self.napelnij(self.z3))
        self.btn_z3_minus = mk_btn("-", x0 + 2 * dx + 145, lambda: self.oproznij(self.z3))

    def przelacz_symulacje(self):
        if self.running:
            self.timer.stop()
        else:
            self.timer.start(20)
        self.running = not self.running

    # --- Zadanie 2: sloty sterowania ---
    def napelnij(self, zb: Zbiornik):
        zb.aktualna_ilosc = zb.pojemnosc
        zb.aktualizuj_poziom()
        self.update()

    def oproznij(self, zb: Zbiornik):
        zb.aktualna_ilosc = 0.0
        zb.aktualizuj_poziom()
        self.update()

    # --- Logika przeplywu ---
    def logika_przeplywu(self):
        # 1) Z1 -> Z2
        plynie_1 = False
        if (not self.z1.czy_pusty()) and (not self.z2.czy_pelny()):
            ilosc = self.z1.usun_ciecz(self.flow_speed)
            self.z2.dodaj_ciecz(ilosc)
            plynie_1 = True
        self.rura1.ustaw_przeplyw(plynie_1)

        # 2) Z2 -> Z3 (startuje dopiero gdy Z2 ma troche wody)
        plynie_2 = False
        if self.z2.aktualna_ilosc > 5.0 and (not self.z3.czy_pelny()):
            ilosc = self.z2.usun_ciecz(self.flow_speed)
            self.z3.dodaj_ciecz(ilosc)
            plynie_2 = True
        self.rura2.ustaw_przeplyw(plynie_2)

        self.update()

    def paintEvent(self, event):
        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing)

        # Najpierw rury (pod spodem), potem zbiorniki
        for r in self.rury:
            r.draw(p)
        for z in self.zbiorniki:
            z.draw(p)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    okno = SymulacjaKaskady()
    okno.show()
    sys.exit(app.exec_())

