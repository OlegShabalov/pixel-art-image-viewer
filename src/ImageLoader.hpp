#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <QObject>
#include <QThread>



class QMovie;
class QPixmap;



class ImageLoadWorker : public QObject {
    Q_OBJECT

public:
    ImageLoadWorker(QObject * parent = nullptr);

public Q_SLOTS:
    void load(const QString & filePath);

Q_SIGNALS:
    void resultReady(QPixmap * image);
};



class ImageLoader : public QObject {
    Q_OBJECT

public:
    ImageLoader(QObject * parent = nullptr);
    ~ImageLoader();

    static bool isImage(const QString & filePath);
    static bool isMovie(const QString & filePath);

    void getResult(QPixmap *& image, QMovie *& movie);
    void load(const QString & filePath);

private Q_SLOTS:
    void _handleResults(QPixmap * image);

Q_SIGNALS:
    void finished();
    void _startLoad(const QString & filePath);

private:
    QMovie * _loadMovie(const QString & filePath) const;
    void _clearResults();

private:
    QThread _thread;

    bool _resultImageNotRelevant = false;
    bool _inProcess = false;

    QString _filePathStack;

    QPixmap * _resultImage = nullptr;
    QMovie * _resultMovie = nullptr;
};



#endif // IMAGELOADER_HPP
