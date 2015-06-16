#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QRegExp>



#include <iostream>
#include <string>

void readVRConfigFile(const std::string& vrConfigFilePath)
{
    QString filename(vrConfigFilePath.c_str());
    QFile f(filename);
    if (!f.exists())
    {
        std::cout << "StreamTracerPlugin::readConfiguration(): Configuration file " << vrConfigFilePath.c_str() << " does not exist!\n";

        return;
    }

    if (!f.open(QIODevice::ReadOnly)) {
        QString errorMessage = f.errorString();
        std::cout << "StreamTracerPlugin::readConfiguration(): Could not open configuration file " << vrConfigFilePath.c_str() << "\n" << errorMessage.toStdString().c_str() << std::endl;
        return;
    }

    QByteArray line;
    unsigned int lineNum = 0;

    // From the Qt documentation:
    // Note: The C++ compiler transforms backslashes in strings.
    // To include a \ in a regexp, enter it twice, i.e. \\.
    // To match the backslash character itself, enter it four times, i.e. \\\\.
    QString	s_comment		("\\s*//");
    QString	s_eye_pos       ("EYE_POSITION");
    QString s_eye_dir		("EYE_DIRECTION");
    QString s_eye_up		("EYE_UP");
    QString s_vf_trans      ("VECTORFIELD_TRANSLATE");
    QString s_vf_scale      ("VECTORFIELD_SCALE");
    QString s_bkg_scene     ("BACKGROUND_SCENE");
    QString s_float			("-*[0-9]*.[0-9]*");
    QString s_name			("[_a-zA-Z0-9.]*");
    QString s_restOfLine	("\\s*\\r?\\n");
    QRegExp rx_emptyLine            (s_restOfLine);
    QRegExp rx_comment              (s_comment);
    QRegExp rx_eye_pos              ("\\s*" + s_eye_pos + "\\s+("	+ s_float + ")\\s+(" + s_float + ")\\s+(" + s_float + ")" + s_restOfLine);
    QRegExp rx_eye_dir              ("\\s*" + s_eye_dir + "\\s+("	+ s_float + ")\\s+(" + s_float + ")\\s+(" + s_float + ")" + s_restOfLine);
    QRegExp rx_eye_up               ("\\s*" + s_eye_up + "\\s+("	+ s_float + ")\\s+(" + s_float + ")\\s+(" + s_float + ")" + s_restOfLine);
    QRegExp rx_vector_field_trans   ("\\s*" + s_vf_trans + "\\s+("	+ s_float + ")\\s+(" + s_float + ")\\s+(" + s_float + ")" + s_restOfLine);
    QRegExp rx_vector_field_scale   ("\\s*" + s_vf_scale + "\\s+("	+ s_float + ")\\s+(" + s_float + ")\\s+(" + s_float + ")" + s_restOfLine);
    QRegExp rx_background_scene     ("\\s*" + s_bkg_scene + "\\s+("	+ s_name + ")" + s_restOfLine);
    while (!f.atEnd())
    {
        line = f.readLine();
        ++lineNum;
        // skip comment lines and empty lines
        if (line.size() == 0 || rx_comment.indexIn(line) == 0 || rx_emptyLine.exactMatch(line))
        {
            continue;
        }
        // built-in types like float or vec3
        else if (rx_eye_pos.indexIn(line) != -1)
        {
            std::cout << "rx_eye_pos " << rx_eye_pos.cap(1).toFloat() << " " << rx_eye_pos.cap(2).toFloat() << " " << rx_eye_pos.cap(3).toFloat() << std::endl;//" " << rx_eye_pos.cap(2).toFloat() << " " << rx_eye_pos.cap(3).toFloat() << std::endl;
        }
        // keywords like for, uniform or struct
        else if (rx_eye_dir.indexIn(line) != -1)
        {
            std::cout << "rx_eye_dir " << rx_eye_dir.cap(1).toFloat() << " " << rx_eye_dir.cap(2).toFloat() << " " << rx_eye_dir.cap(3).toFloat() << std::endl;
        }
        // built-in functions like cross or smoothstep
        else if (rx_eye_up.indexIn(line) != -1)
        {
            std::cout << "rx_eye_up " << rx_eye_up.cap(1).toFloat() <<" " << rx_eye_up.cap(2).toFloat() << " " << rx_eye_up.cap(3).toFloat() << std::endl;
        }
        else if (rx_vector_field_trans.indexIn(line) != -1)
        {
            std::cout << "rx_vector_field_trans " << rx_vector_field_trans.cap(1).toFloat() << " " << rx_vector_field_trans.cap(2).toFloat() << " " << rx_vector_field_trans.cap(3).toFloat() << std::endl;
        }
        else if (rx_vector_field_scale.indexIn(line) != -1)
        {
            std::cout << "rx_vector_field_scale " << rx_vector_field_scale.cap(1).toFloat() << " " << rx_vector_field_scale.cap(2).toFloat() << " " << rx_vector_field_scale.cap(3).toFloat() << std::endl;
        }
        else if (rx_background_scene.indexIn(line) != 1)
        {
            std::cout << "Scene Name = " << rx_background_scene.cap(1).toStdString() << std::endl;
        }
        else
        {
            std::cout << line.data() << std::endl;
            std::cout << " StreamTracerPlugin::readVRConfigFile: Error parsing configuration file " << vrConfigFilePath.c_str() << " at line " << lineNum << ".\n";
        }
    }

    f.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    readVRConfigFile("./test.cfg");

    std::cout << "Done\n";

    return a.exec();
}
