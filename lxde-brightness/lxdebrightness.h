#ifndef LXDEBRIGHTNESS_H
#define LXDEBRIGHTNESS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LXDEBrightness; }
QT_END_NAMESPACE

class LXDEBrightness : public QMainWindow
{
    Q_OBJECT

public:
    LXDEBrightness(QWidget *parent = nullptr);
    ~LXDEBrightness();

private slots:
    void on_brightness_valueChanged(int value);
    void on_display_output_currentIndexChanged(int index);

private:
    Ui::LXDEBrightness *ui;

    /**
     * @brief function that gets display output list and fill them into combobox
     * @return if there is no display output or any error, return false. Otherwise, return true.
     */
    bool getDisplayOutputs( void );

    /**
     * @brief function that gets brightness value of current display output and sets brightness slider with the value
     * @return if the brightness is set, return true. Otherwise, return false.
     */
    bool setBrightness( void );
};

#endif
