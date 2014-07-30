#ifndef FOTO_RESIZE_H
#define FOTO_RESIZE_H

#include <QtGui/QMainWindow>
#include "ui_foto_resize.h"

#include <QDir>

class Foto_resize : public QMainWindow
{
	Q_OBJECT

public:
	Foto_resize(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Foto_resize();

private:
	Ui::Foto_resizeClass ui;
	void resizeImage(QDir currentDir, QString filename);

protected slots:
	void pbInputFolderRename_clicked();
	void pbOutputFolderRename_clicked();
	void pbRename_clicked();

	void pbAddFolders_clicked();
	void pbRemoveFolder_clicked();
	void pbResize_clicked();
	void pbOutputFolder_clicked();

};

#endif // FOTO_RESIZE_H
