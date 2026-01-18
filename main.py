import sys
from PyQt5.QtWidgets import QApplication
from okno_glowne import OknoGlowne

def main():
    app = QApplication(sys.argv)
    okno = OknoGlowne()
    okno.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
