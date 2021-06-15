#ifndef QCLOCKPROGRESS_H_
#define QCLOCKPROGRESS_H_

#include <QWidget>
#include <QTimer>

class QClockProgress : public QWidget
{
        Q_OBJECT

    public:
        enum Format {Percentage, Fraction, UserType};
        /******************************
         * Function: QClockProgress(QWidget *parent = 0)
         * Description: Construct clock progress widget.
         * Input: parent	QWidget *	parent widget
         * Output: parent	QWidget *	parent widget
         * Return: void
         ******************************/
        QClockProgress(QWidget *parent = 0);

        /******************************
         * Function: ~QClockProgress()
         * Description: Destroy clock progress widget.
         * Input: void
         * Output: void
         * Return: void
         ******************************/
        ~QClockProgress();

        /******************************
         * Function: start()
         * Description: Start clock progress,
         * 				you must start it, otherwise is invisible.
         * Input: void
         * Output: void
         * Return: void
         ******************************/
        void start();

        /******************************
         * Function: stop()
         * Description: Stop clock progress and it's invisible.
         * Input: void
         * Output: void
         * Return: void
         ******************************/
        void stop();

        /******************************
         * Function: setFormat(Format format = UserType)
         * Description: Set clock progress text format.
         * Input: format		Format		text format
         * Output: void
         * Return: void
         ******************************/
        void setFormat(Format format = UserType);

        /******************************
         * Function: setRange(int min, int max)
         * Description: Set range.
         * Input: min	int		min value
         * 		max		int		max value
         * Output: void
         * Return: void
         ******************************/
        void setRange(int min, int max);

        /******************************
         * Function: setValue(int value)
         * Description: Set range.
         * Input: value		int		current value
         * Output: void
         * Return: void
         ******************************/
        void setValue(int value);

    private slots:
        /******************************
         * Function: roll()
         * Description: Draw runing clock.
         * Input: void
         * Output: void
         * Return: void
         ******************************/
        void roll();

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        QTimer *m_pTimer;
        bool bIsTextVisible;
        Format m_format;
        int m_maxValue;
        int m_minValue;
        int m_curValue;
};

#endif /* CLOCKPROGRESS_H_ */
