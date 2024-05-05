#include "Application.hpp"

#include <QFileInfo>
#include <QDir>
#include <QCollator>

#include "src/widgets/MainWidget.hpp"

#include <QApplication>



Application::Application() {
    QStringList args = QApplication::arguments();
    int index = 0;

    if (args.size() == 2) {
        _loadImageList(args.at(1));
        index = reloadImageList(args.at(1));
    }

    MainWidget * widget = new MainWidget(*this, index);
    widget->show();
}



QString Application::imagePathName(int index) const {
    if (index < _imageNames.size()) {
        return QDir(_dirPath).filePath(_imageNames.at(index));
    }
    return QString();
}

QString Application::imageName(int index) const {
    if (index < _imageNames.size()) {
        return _imageNames.at(index);
    }
    return QString();
}



int Application::nextIndex(int index) const {
    if (index == _imageNames.size() - 1) {
        return 0;
    }
    return index + 1;
}

int Application::previousIndex(int index) const {
    if (index == 0) {
        return _imageNames.size() - 1;
    }
    return index - 1;
}



int Application::reloadImageList(const QString & filePath) {
    _loadImageList(filePath);

    int index = 0;

    int fileIndex = -1;
    const QString fileName = QFileInfo(filePath).fileName();
    for (int i=0; i<_imageNames.size(); ++i) {
        if (fileName == _imageNames.at(i)) {
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1) {
        _imageNames.push_front(fileName);
    }
    else {
        index = fileIndex;
    }

    return index;
}

int Application::reloadImageList(const QStringList & filesPath) {
    if (filesPath.size() == 1) {
        return reloadImageList(filesPath.first());
    }

    const QFileInfo currentFile(filesPath.first());
    _dirPath = currentFile.absolutePath();

    _imageNames.clear();
    for (int i=0; i<filesPath.size(); ++i) {
        _imageNames.append(QFileInfo(filesPath.at(i)).fileName());
    }

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(_imageNames.begin(), _imageNames.end(), collator);

    return 0;
}



void Application::_loadImageList(const QString & filePath) {
    QStringList filters;
    filters << "*.JPEG" << "*.JPG" << "*.PNG" << "*.BMP" << "*.GIF" <<
               "*.PBM" << "*.PGM" << "*.PPM" << "*.XBM" << "*.XPM" << "*.SVG";

    const QFileInfo currentFile(filePath);
    _dirPath = currentFile.absolutePath();

    const QDir dir(_dirPath);
    _imageNames = dir.entryList(filters);

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(_imageNames.begin(), _imageNames.end(), collator);
}
