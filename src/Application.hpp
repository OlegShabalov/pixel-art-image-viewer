#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QObject>



class Application : public QObject {
    Q_OBJECT

public:
    Application();

    QString imagePathName(int index) const;
    QString imageName(int index) const;

    int nextIndex(int index) const;
    int previousIndex(int index) const;
    inline int imageListSize() const;
    inline bool imageListEmpty() const;

    int reloadImageList(const QString & filePath);
    int reloadImageList(const QStringList & filesPath);

private:
    void _loadImageList(const QString & filePath);

private:
    QStringList _imageNames;
    QString _dirPath;
};



inline int Application::imageListSize() const {
    return _imageNames.size();
}

inline bool Application::imageListEmpty() const {
    return _imageNames.empty();
}



#endif // APPLICATION_HPP
