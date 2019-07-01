#include "mainwindow.h"
#include "songdata.h"
#include "songview.h"
#include <memory>
#include <QApplication>
#include <QLabel>

void fillParty(SongData::Party& party)
{
   using namespace SongData;

   party.clefType = ClefType::GClef;
   party.elements.push_back(std::unique_ptr<BaseElement>(new BarSize(4, 4)));

   Chord* c =  new Chord;
   c->addNote(42);    // #F, 6+3*12
   c->addFlag(Flags::StacattoTenuto);

   party.elements.push_back(std::unique_ptr<BaseElement>(c));

   c = new Chord;
   c->addNote(45);   // A,  9+3*12
   c->addNote(41);
   c->addFlag(Flags::AccentuatedNote);
   party.elements.push_back(std::unique_ptr<BaseElement>(c));

   c = new Chord(Duration(2));
   c->addNote(43);   // G, 7+3*12
   party.elements.push_back(std::unique_ptr<BaseElement>(c));

   party.elements.push_back(std::unique_ptr<BaseElement>(new BarLine(false, false, true)));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SongData::Party party;
    fillParty(party);

    bildSongView(party, w);

    return a.exec();
}
