#include "foto_resize.h"
#include <QFileDialog>
#include <QPixmap>
#include <time.h>

Foto_resize::Foto_resize(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.lResize1->setVisible(false);
	ui.lResize2->setVisible(false);
	ui.lRename->setVisible(false);

	connect(ui.pbInputFolderRename, SIGNAL(clicked()),this, SLOT(pbInputFolderRename_clicked()));
	connect(ui.pbOutputFolderRename, SIGNAL(clicked()),this, SLOT(pbOutputFolderRename_clicked()));
	connect(ui.pbRename, SIGNAL(clicked()),this, SLOT(pbRename_clicked()));

	connect(ui.pbAddFolders, SIGNAL(clicked()),this, SLOT(pbAddFolders_clicked()));
	connect(ui.pbRemoveFolder, SIGNAL(clicked()),this, SLOT(pbRemoveFolder_clicked()));

	connect(ui.pbResize, SIGNAL(clicked()),this, SLOT(pbResize_clicked()));
	connect(ui.pbOutputFolder, SIGNAL(clicked()),this, SLOT(pbOutputFolder_clicked()));

}

Foto_resize::~Foto_resize()
{

}

void Foto_resize::pbInputFolderRename_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             "/home",
                                             QFileDialog::ShowDirsOnly /*|
                                              QFileDialog::DontResolveSymlinks*/);

	ui.leInputFolderRename->setText(dir);

}

void Foto_resize::pbOutputFolderRename_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             "/home",
                                             QFileDialog::ShowDirsOnly /*|
                                              QFileDialog::DontResolveSymlinks*/);

	ui.leOutputFolderRename->setText(dir);

}

void Foto_resize::pbRename_clicked()
{
	QDir currentDir(ui.leInputFolderRename->text());

	QStringList files;

	QStringList filters;
	filters << "*.jpeg" << "*.jpg" << "*.png";

	currentDir.setNameFilters(filters);

	files = currentDir.entryList(QDir::Files | QDir::NoSymLinks);
	//files = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

	QString file_ext;
	srand (time(NULL));
	QVector <int> numbers;
	numbers.clear();

	ui.lRename->setVisible(true);
	for (int j = 0; j < files.size(); j++) 
	{
		ui.lRename->setText(QString("num of files: %1").arg(j + 1));

		qApp->processEvents();

		int name_int;
		QString name;
		name_int = (int)(((double)rand()/(double)RAND_MAX) * 4000);

		while(numbers.contains(name_int))
		{
			name_int = (int)(((double)rand()/(double)RAND_MAX) * 4000);
		}

		numbers.push_back(name_int);


		file_ext = files[j];
		file_ext = file_ext.remove(0, files[j].lastIndexOf("."));

		if(name_int < 10)
		{
			name = QString("000") + QString::number(name_int) + QString(file_ext);
		}
		else if(name_int < 100)
		{
			name = QString("00") + QString::number(name_int) + QString(file_ext);
		}
		else if(name_int < 1000)
		{
			name = QString("0") + QString::number(name_int) + QString(file_ext);
		}
		else
		{
			name = QString::number(name_int) + QString(file_ext);
		}

	
		QFile file(currentDir.absoluteFilePath(files[j]));

		if(file.exists())
			file.rename(QString(ui.leOutputFolderRename->text()).append("/").append(name));
	}

	ui.lRename->setVisible(false);
}

void Foto_resize::pbResize_clicked()
{
	ui.lResize1->setVisible(true);
	ui.lResize2->setVisible(true);

	for(int i = 0; i < ui.lwFolderList->count(); i++)
	{
		ui.lResize1->setText(QString("num of directories: %1").arg(ui.lwFolderList->count()) + QString(" curr directory: %1").arg(i + 1));
		
		QDir currentDir(ui.lwFolderList->item(i)->text());

		QStringList files;

		QStringList filters;
		filters << "*.jpeg" << "*.jpg" << "*.JPG" << "*.JPEG" << "*.png";

		currentDir.setNameFilters(filters);

		files = currentDir.entryList(QDir::Files | QDir::NoSymLinks);
		//files = currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);

		for (int j = 0; j < files.size(); j++) 
		{
			ui.lResize2->setText(QString("num of files: %1").arg(files.size()) + QString(" curr file: %1").arg(j + 1));
			qApp->processEvents();
			//QString a = files[j].remove(0, files[j].lastIndexOf(".") + 1);
			resizeImage(currentDir, files[j]);
		}
	}

	ui.lResize1->setVisible(false);
	ui.lResize2->setVisible(false);

}

void Foto_resize::resizeImage(QDir currentDir, QString filename)
{
	//QFile file(currentDir.absoluteFilePath(filename));
	//if (file.open(QIODevice::ReadOnly))
	{
		double w;
		double h;
		w = ui.leWidth->text().toInt();
		h = ui.leHeight->text().toInt();

		double reference_ratio;
		double ratio;
		QPixmap origPixmap;

		reference_ratio = w / h;
		if(origPixmap.load(currentDir.absoluteFilePath(filename)) == false)
		{
			printf("error open file\n");
		}
		ratio = (double)origPixmap.width() / (double)origPixmap.height();

		int condition;

		if((w / ratio) > h)
		{
			condition = 1;
		}
		else
		{
			condition = 2;
		}

		if((origPixmap.width() > origPixmap.height())&& (reference_ratio == ratio)) 
		{
			QPixmap scaledPixmap(origPixmap.scaled(QSize(w, h)));
			scaledPixmap.save(QString(ui.leOutputFolderResize->text()).append("\\").append(filename));
		}
		else if((origPixmap.width() > origPixmap.height()) && (condition == 2)/*(reference_ratio != ratio)*/)
		{
			QPixmap scaledPixmap(origPixmap.scaled(QSize(w, w / ratio)));
			QPixmap blackPix(w, h);

			int start_pixel;
			start_pixel = (h - (origPixmap.height() * w / origPixmap.width())) / 2.0;

			QImage out_img;
			QImage in_img;

			out_img = blackPix.toImage();
			in_img = scaledPixmap.toImage();

			for(int j = 0; j < scaledPixmap.height(); j++)
			{
				for(int i = 0; i < scaledPixmap.width(); i++)
				{
					out_img.setPixel(i, j + start_pixel, in_img.pixel(i, j));
				}
			}
			blackPix = QPixmap::fromImage(out_img);
			blackPix.save(QString(ui.leOutputFolderResize->text()).append("\\").append(filename));
		}
		else
		{
			QPixmap scaledPixmap(origPixmap.scaled(QSize(ratio * h, h)));
			QPixmap blackPix(w, h);

			int start_pixel;
			start_pixel = (w - (origPixmap.width() * h / origPixmap.height())) / 2.0;

			QImage out_img;
			QImage in_img;

			out_img = blackPix.toImage();
			in_img = scaledPixmap.toImage();
			for(int j = 0; j < scaledPixmap.height(); j++)
			{
				for(int i = 0; i < scaledPixmap.width(); i++)
				{
					out_img.setPixel(start_pixel + i, j, in_img.pixel(i, j));
				}
			}
			blackPix = QPixmap::fromImage(out_img);
			blackPix.save(QString(ui.leOutputFolderResize->text()).append("\\").append(filename));
		}
		
		
	}
}

void Foto_resize::pbAddFolders_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             "/home",
                                             QFileDialog::ShowDirsOnly |
                                              QFileDialog::DontResolveSymlinks);

	new QListWidgetItem(dir, ui.lwFolderList);
}

void Foto_resize::pbOutputFolder_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             "/home",
                                             QFileDialog::ShowDirsOnly |
                                              QFileDialog::DontResolveSymlinks);

	ui.leOutputFolderResize->setText(dir);
	
}

void Foto_resize::pbRemoveFolder_clicked()
{
	QListWidgetItem* i = NULL;
	i = ui.lwFolderList->selectedItems()[0];

	if(i != NULL)
		delete i;

	ui.lwFolderList->update();
}