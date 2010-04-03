
#include <cwiid.h>

#include <QtGui>
#include <kdebug.h>



void connectWiimote()
{
    bdaddr_t bdaddr;
    char reset_bdaddr = 0;

    bacmp(&bdaddr, BDADDR_ANY);
    //if (bacmp(&bdaddr, BDADDR_ANY) == 0) {
    //    reset_bdaddr = 1;
    //}
    //message(GTK_MESSAGE_INFO,
    //        "Put Wiimote in discoverable mode (press 1+2) and press OK",
    //         GTK_WINDOW(winMain));

    kDebug() << "Put Wiimote in discoverable mode (press 1+2) and press OK";
    wiimote = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC)
    if (!wiimote) {
        //message(GTK_MESSAGE_ERROR, "Unable to connect", GTK_WINDOW(winMain));
        //status("No connection");
        kDebug() << "Unable to connect";
    } else if (cwiid_set_mesg_callback(wiimote, &cwiid_callback)) {
        //message(GTK_MESSAGE_ERROR, "Error setting callback",
        //        GTK_WINDOW(winMain));
        kDebug() << "Error setting callback";
        if (cwiid_close(wiimote)) {
            //message(GTK_MESSAGE_ERROR, "Error on disconnect",
             //       GTK_WINDOW(winMain));
            kDebug() << "Error on disconnect";
        }
        wiimote = NULL;
        //status("No connection");
        kDebug() << "status: no connection";
    }
    else {
        kDebug() << "status connected";
        status("Connected");
        if (cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &wm_cal)) {
            //message(GTK_MESSAGE_ERROR, "Unable to retrieve accelerometer "
            //        "calibration", GTK_WINDOW(winMain));
            kDebug() << "Unable to retrieve accelerometer calibration";
        }
        //set_gui_state();
        //set_report_mode();
        cwiid_request_status(wiimote);
    }

    if (reset_bdaddr) {
        bdaddr = *BDADDR_ANY;
    }
    kDebug() << "Done. :)";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int c;
    uint8_t flags = 0;
    char long_format = 0;
    char quiet = 0;
    struct cwiid_bdinfo *bdinfo;
    int bdinfo_count;
    int i;
    char ba_str[18];

    if ((bdinfo_count = cwiid_get_bdinfo_array(-1, 2, -1, &bdinfo, flags))
      == -1) {
        return -1;
    }
    cwiid_wiimote_t *wiimote = NULL;

    /* Print info */
    for (i=0; i < bdinfo_count; i++) {
        ba2str(&bdinfo[i].bdaddr, ba_str);
        //if (long_format) {
            //printf("%s 0x%.2X%.2X%.2X %s\n", ba_str, bdinfo[i].btclass[2],
            //       bdinfo[i].btclass[1], bdinfo[i].btclass[0], bdinfo[i].name);
        //}
        //else {
            printf("%s\n", ba_str);
            QString baString(ba_str);
            kDebug() << "BA:" << baString;
        //}
    }
    connectWiimote();
    qDebug() << "bla";
    return app.exec();
}
