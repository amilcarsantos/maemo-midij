#include "MidijDraw.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MidiJDraw w;
	w.show();

	//w.setCurrentProject("/Wascana/workspace/MidiJ/data/schemes/collusion", "midijGui_Collusion.xml");


	//w.drawHotWheelsSkin();
//	w.setCurrentProject("/Wascana/workspace/MidiJ/data/schemes/blue", "midijGui_Blue.xml",
//			"../stantonscs3d/stanton SCS.3d.midi.xml", "StantonSCS3d");

	//w.drawDados6();

	w.drawHwBrowsingSkin();

	return a.exec();
}
