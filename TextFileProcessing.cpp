#include "fstream"

namespace textWorker {
    void openInFile(std::ifstream fin, char fileName[], std::ios_base::openmode mode)   //opening modes:
    {                                                                                   //ios_base::in  allow input operations on the stream (file)
        fin.open(fileName, mode);                                                       //ios_base::out allow output operations on the stream
    }                                                                                   //ios_base::ate set the stream's position indicator to the end of the stream on opening.
                                                                                        //ios_base::app set the stream's position indicator to the end of the stream before each output operation
    void openOutFile(std::ofstream fout, char fileName[], std::ios_base::openmode mode) //ios_base::binary consider stream (file) as binary rather than text
    {                                                                                   //ios_base::trunc any current content is discarded, assuming a length of zero on opening
                                                                                        //file opening modes can be combined using "|", for example ios_base::out | ios_base::trunc
        fout.open(fileName, mode);                                                      //default ifstream: ios_base::in
    }                                                                                   //default ofstream: ios_base::out | ios_base::trunc

    bool checkInFileEx(std::ifstream fin) //after using openFile
    {
        return fin.is_open();
    }

    bool checkOutFileEx(std::ofstream fout)
    {
        return fout.is_open();      //same
    }

    bool checkIfEndOfInFileWasReached(std::ifstream fin)
    {
        return fin.eof();
    }

    int tellHowMuchRead(std::ifstream fin) //[tellHowMuchRead] = bytes
    {
        return fin.tellg();
    }

    void readFromFile(std::ifstream fin, char buffer[]) //alternative: count number of characters in buffer/output
    {
        fin >> buffer;
    }

    void writeInFile(std::ofstream fout, char output[])
    {
        fout << output;
    }

    void closeInFile(std::ifstream fin)
    {
        fin.close();
    }

    void closeOutFile(std::ofstream fout)
    {
        fout.close();
    }

}
