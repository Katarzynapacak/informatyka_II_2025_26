"""
okno_glowne.py
GUI: ekran instalacji + alarmy + trendy (matplotlib) + dialog startowy.

Poprawka: zakładki (Instalacja/Alarmy/Trendy) i tabela alarmów mają jasne tło
oraz czarny tekst, żeby wszystko było czytelne.
"""

from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtWidgets import (
    QWidget, QMainWindow, QTabWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QFormLayout, QDialog, QDialogButtonBox,
    QDoubleSpinBox, QCheckBox, QTableWidget, QTableWidgetItem
)

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as Canvas
from matplotlib.figure import Figure

from model import ModelInstalacji


class DialogStartowy(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Parametry startowe")

        self.z1 = QDoubleSpinBox(); self.z1.setRange(0, 100); self.z1.setValue(80)
        self.z2 = QDoubleSpinBox(); self.z2.setRange(0, 100); self.z2.setValue(10)
        self.z3 = QDoubleSpinBox(); self.z3.setRange(0, 100); self.z3.setValue(20)
        self.z4 = QDoubleSpinBox(); self.z4.setRange(0, 100); self.z4.setValue(10)

        self.predkosc = QDoubleSpinBox(); self.predkosc.setRange(0.2, 2.0)
        self.predkosc.setSingleStep(0.1); self.predkosc.setValue(1.0)

        self.moc = QDoubleSpinBox(); self.moc.setRange(0.0, 6.0)
        self.moc.setSingleStep(0.5); self.moc.setValue(3.0)

        self.temp = QDoubleSpinBox(); self.temp.setRange(0, 120); self.temp.setValue(20)

        self.pompa_on = QCheckBox("Pompa ON"); self.pompa_on.setChecked(True)
        self.grzalka_on = QCheckBox("Grzałka ON"); self.grzalka_on.setChecked(True)

        form = QFormLayout()
        form.addRow("Z1 [%]:", self.z1)
        form.addRow("Z2 [%]:", self.z2)
        form.addRow("Z3 [%]:", self.z3)
        form.addRow("Z4 [%]:", self.z4)
        form.addRow("Prędkość pompy:", self.predkosc)
        form.addRow("Moc grzałki:", self.moc)
        form.addRow("Temp start [°C]:", self.temp)
        form.addRow(self.pompa_on)
        form.addRow(self.grzalka_on)

        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.accepted.connect(self.accept)
        buttons.rejected.connect(self.reject)

        lay = QVBoxLayout()
        lay.addLayout(form)
        lay.addWidget(buttons)
        self.setLayout(lay)

    def pobierz(self):
        return dict(
            z1_proc=self.z1.value(),
            z2_proc=self.z2.value(),
            z3_proc=self.z3.value(),
            z4_proc=self.z4.value(),
            predkosc_pompy=self.predkosc.value(),
            moc_grzalki=self.moc.value(),
            pompa_on=self.pompa_on.isChecked(),
            grzalka_on=self.grzalka_on.isChecked(),
            temp_start=self.temp.value()
        )


class EkranInstalacji(QWidget):
    def __init__(self, model: ModelInstalacji):
        super().__init__()
        self.model = model
        self.setMinimumSize(760, 520)

    def paintEvent(self, e):
        p = QPainter(self)
        p.setRenderHint(QPainter.Antialiasing)
        p.fillRect(self.rect(), QColor(34, 34, 34))

        # rury pod spodem
        for r in self.model.rury:
            r.rysuj(p)

        # elementy
        for z in self.model.zbiorniki:
            z.rysuj(p)

        self.model.pompa.rysuj(p)
        self.model.grzalka.rysuj(p)


class EkranAlarmow(QWidget):
    def __init__(self, model: ModelInstalacji):
        super().__init__()
        self.model = model

        self.tabela = QTableWidget(0, 3)
        self.tabela.setHorizontalHeaderLabels(["Tag", "Opis", "Stan"])
        self.tabela.verticalHeader().setVisible(False)

        # Czytelność: czarny tekst na białym tle + jasne nagłówki
        self.tabela.setStyleSheet("""
            QTableWidget {
                background-color: white;
                color: black;
                gridline-color: #aaa;
            }
            QHeaderView::section {
                background-color: #eee;
                color: black;
                padding: 4px;
                border: 1px solid #bbb;
            }
        """)

        lay = QVBoxLayout()
        lay.addWidget(self.tabela)
        self.setLayout(lay)

    def odswiez(self):
        self.tabela.setRowCount(len(self.model.alarmy))
        for i, a in enumerate(self.model.alarmy):
            self.tabela.setItem(i, 0, QTableWidgetItem(a.tag))
            self.tabela.setItem(i, 1, QTableWidgetItem(a.opis))
            stan = "AKTYWNY" if a.aktywny else "OK"
            self.tabela.setItem(i, 2, QTableWidgetItem(stan))

        self.tabela.resizeColumnsToContents()


class EkranTrendy(QWidget):
    def __init__(self, model: ModelInstalacji):
        super().__init__()
        self.model = model

        self.fig = Figure()
        self.canvas = Canvas(self.fig)
        self.ax = self.fig.add_subplot(111)

        lay = QVBoxLayout()
        lay.addWidget(self.canvas)
        self.setLayout(lay)

    def odswiez(self):
        if not self.model.historia:
            return

        t = [x[0] for x in self.model.historia]
        z1 = [x[1] * 100 for x in self.model.historia]
        z2 = [x[2] * 100 for x in self.model.historia]
        z3 = [x[3] * 100 for x in self.model.historia]
        z4 = [x[4] * 100 for x in self.model.historia]
        T3 = [x[5] for x in self.model.historia]

        self.ax.clear()
        self.ax.plot(t, z1, label="Z1 [%]")
        self.ax.plot(t, z2, label="Z2 [%]")
        self.ax.plot(t, z3, label="Z3 [%]")
        self.ax.plot(t, z4, label="Z4 [%]")
        self.ax.plot(t, T3, label="T3 [°C]")
        self.ax.set_xlabel("t [s]")
        self.ax.grid(True)
        self.ax.legend(loc="upper right")
        self.canvas.draw_idle()


class OknoGlowne(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Mini SCADA - instalacja 4 zbiorniki")
        self.setStyleSheet("background-color: #222; color: white;")

        self.model = ModelInstalacji()

        # dialog startowy
        dlg = DialogStartowy(self)
        if dlg.exec_() == QDialog.Accepted:
            self.model.ustaw_parametry_startowe(**dlg.pobierz())

        self.tabs = QTabWidget()

        # Czytelność zakładek: czarny tekst na jasnym tle
        self.tabs.setStyleSheet("""
            QTabWidget::pane {
                border: 1px solid #444;
                background: #222;   /* tło pod zakładkami */
            }
            QTabBar::tab {
                background: #f2f2f2;
                color: black;
                padding: 6px 12px;
                border: 1px solid #bbb;
                border-bottom: none;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background: white;
                font-weight: bold;
            }
        """)

        self.ekran_inst = EkranInstalacji(self.model)
        self.ekran_alarm = EkranAlarmow(self.model)
        self.ekran_trend = EkranTrendy(self.model)

        self.tabs.addTab(self.ekran_inst, "Instalacja")
        self.tabs.addTab(self.ekran_alarm, "Alarmy")
        self.tabs.addTab(self.ekran_trend, "Trendy")

        # sterowanie
        self.btn_start = QPushButton("Start/Stop")
        self.btn_start.clicked.connect(self.przelacz)

        top = QWidget()
        lay = QVBoxLayout(top)
        bar = QHBoxLayout()
        bar.addWidget(self.btn_start)
        bar.addStretch(1)
        lay.addLayout(bar)
        lay.addWidget(self.tabs)
        self.setCentralWidget(top)

        # timer symulacji
        self.dt = 0.02
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.krok)
        self._run = False

    def przelacz(self):
        self._run = not self._run
        if self._run:
            self.timer.start(int(self.dt * 1000))
        else:
            self.timer.stop()

    def krok(self):
        self.model.krok(self.dt)
        self.ekran_inst.update()
        self.ekran_alarm.odswiez()
        self.ekran_trend.odswiez()
