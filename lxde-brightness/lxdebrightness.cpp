#include "lxdebrightness.h"
#include "ui_lxdebrightness.h"
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>

//! digit count of brightness value from OS
#define BRIGHTNESS_DIGIT (8)

//! maximum screen name length
#define MAX_SCREEN_NAME_LEN (16)

LXDEBrightness::LXDEBrightness(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LXDEBrightness)
{
    ui->setupUi(this);
    ui->display_output->clear();

    // if there is display output(s), list them
    if (getDisplayOutputs()) {
        // set brightness slider with brightness value of first display output into the combobox
        setBrightness();

        connect(ui->brightness, SIGNAL(valueChanged(int)), this, SLOT(on_brightness_valueChanged(int)));
        connect(ui->display_output, SIGNAL(currentIndexChanged(int)), this, SLOT(on_display_output_currentIndexChanged(int)));
    }
    else {
        ui->display_output->setEnabled(false);
        ui->brightness->setEnabled(false);

        this->setWindowTitle("No display output is detected");
    }
}

LXDEBrightness::~LXDEBrightness()
{
    delete ui;
}

bool LXDEBrightness::setBrightness( void )
{
    char raw_value[BRIGHTNESS_DIGIT] = { 0x00 };
    FILE *fPtr = NULL;
    QString cmd;

    cmd = "xrandr --verbose --current | grep " + ui->display_output->currentText() + " -A5 | grep -i brightness | cut -d \" \" -f2";

  if (NULL == (fPtr = popen(cmd.toStdString().c_str(), "r"))) {
      return false;
  }

  memset(raw_value, 0x00, BRIGHTNESS_DIGIT);

  if (NULL != fgets(raw_value, BRIGHTNESS_DIGIT, fPtr)) {
      qint8 eng_value = (qint8) (QString(raw_value).trimmed().toFloat() * 100.0f);

      ui->brightness->setValue(eng_value);
      ui->brightness->setToolTip(QString::number(eng_value));
  }

  pclose(fPtr);

  return true;
}

void LXDEBrightness::on_brightness_valueChanged(int value)
{
    float xrandr_val = (float) value / 100.0f;
    FILE *fPtr = NULL;
    QString cmd;

    ui->brightness->setValue(value);
    ui->brightness->setToolTip(QString::number(value));

    cmd = "xrandr --output " + ui->display_output->currentText() + " --brightness " + QString::number( xrandr_val );

    fPtr = popen(cmd.toStdString().c_str(), "r");
    fclose(fPtr);

    return;
}

bool LXDEBrightness::getDisplayOutputs(void) {
    char raw_value[MAX_SCREEN_NAME_LEN] = { 0x00 };
    FILE *fPtr = NULL;

    if (NULL == (fPtr = popen("xrandr | grep -w connected | cut -d \" \" -f1", "r"))) {
        return false;
    }

    memset(raw_value, 0x00, MAX_SCREEN_NAME_LEN);

    while (NULL != fgets(raw_value, MAX_SCREEN_NAME_LEN, fPtr)) {
        ui->display_output->addItem(QString(raw_value).trimmed());
        memset(raw_value, 0x00, MAX_SCREEN_NAME_LEN);
    }

    pclose(fPtr);

    return true;
}

void LXDEBrightness::on_display_output_currentIndexChanged(int index)
{
    // if the display output is changed, set its value instead of current value
    setBrightness();

    return;
}
