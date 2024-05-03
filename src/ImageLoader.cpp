#include "ImageLoader.hpp"

#include <QFileInfo>
#include <QPixmap>
#include <QMovie>



ImageLoadWorker::ImageLoadWorker(QObject * parent)
    : QObject(parent)
{}

void ImageLoadWorker::load(const QString & filePath) {
    QPixmap * image = new QPixmap;

    if (!image->load(filePath)) {
        delete image;
        image = nullptr;
    }

    Q_EMIT resultReady(image);
}



ImageLoader::ImageLoader(QObject * parent)
    : QObject(parent)
{
    ImageLoadWorker * worker = new ImageLoadWorker;
    worker->moveToThread(&_thread);

    connect(&_thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ImageLoader::_startLoad, worker, &ImageLoadWorker::load);
    connect(worker, &ImageLoadWorker::resultReady,
            this, &ImageLoader::_handleResults);

    _thread.start();
}

ImageLoader::~ImageLoader() {
    _thread.quit();
    _thread.wait();
}



bool ImageLoader::isImage(const QString & filePath) {
    const QString suffix = QFileInfo(filePath).suffix();
    return (QString::compare(suffix, "PNG",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "JPG",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "JPEG", Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "BMP",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "PBM",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "PGM",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "PPM",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "XBM",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "XBM",  Qt::CaseInsensitive) == 0 ||
            QString::compare(suffix, "SVG",  Qt::CaseInsensitive) == 0);
}

bool ImageLoader::isMovie(const QString & filePath) {
    const QString suffix = QFileInfo(filePath).suffix();
    return (QString::compare(suffix, "GIF", Qt::CaseInsensitive) == 0);
}



void ImageLoader::getResult(QPixmap *& image, QMovie *& movie) {
    image = _resultImage;
    movie = _resultMovie;
    _resultImage = nullptr;
    _resultMovie = nullptr;
}

void ImageLoader::load(const QString & filePath) {
    if (_inProcess) {
        if (QFileInfo::exists(filePath)) {
            if (isImage(filePath)) {
                _resultImageNotRelevant = true;
                _filePathStack = filePath;
                return;
            }
            if (isMovie(filePath)) {
                _resultImageNotRelevant = true;
                _clearResults();
                _resultMovie = _loadMovie(filePath);
                Q_EMIT finished();
                return;
            }
        }

        _resultImageNotRelevant = true;
        _clearResults();
        Q_EMIT finished();
    }
    else {
        if (QFileInfo::exists(filePath)) {
            if (isImage(filePath)) {
                _inProcess = true;
                Q_EMIT _startLoad(filePath);
                return;
            }
            if (isMovie(filePath)) {
                _clearResults();
                _resultMovie = _loadMovie(filePath);
                Q_EMIT finished();
                return;
            }
        }

        _clearResults();
        Q_EMIT finished();
    }
}

void ImageLoader::_handleResults(QPixmap * image) {
    if (_filePathStack.isEmpty()) {
        _inProcess = false;

        if (_resultImageNotRelevant) {
            _resultImageNotRelevant = false;
            if (image) delete image;
        }
        else {
            _clearResults();
            _resultImage = image;
            Q_EMIT finished();
        }
    }
    else {
        _resultImageNotRelevant = false;
        if (image) delete image;

        const QString filePath(qMove(_filePathStack));
        _filePathStack.clear();

        Q_EMIT _startLoad(filePath);
    }
}

QMovie * ImageLoader::_loadMovie(const QString & filePath) const {
    QMovie * result = new QMovie(filePath);

    if (!result->isValid()) {
        delete result;
        result = nullptr;
    }

    return result;
}

void ImageLoader::_clearResults() {
    if (_resultImage) {
        delete _resultImage;
        _resultImage = nullptr;
    }
    if (_resultMovie) {
        delete _resultMovie;
        _resultMovie = nullptr;
    }
}
