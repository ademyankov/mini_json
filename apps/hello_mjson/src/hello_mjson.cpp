#include "mjson/mjson.hpp"

int main() {
    const auto config = R"(
        {
            "Device"    : "HeartMN1",
            "ID"        : "PAMF-0119239.A1.PBS-09",
            "Class"     : "Monitor",
            "Type"      : "Wearable",

            "Firmware" : {
                "Version" : "1.123.900",
                "Image"   : [ "PBS-09", "PBS-10", "PBS-10.A", "PBS-11" ],

                "Update" : {
                    "Server"        : "https://update.firmware.com:8774/release",
                    "Connection"    : "mTLS",
                    "Client.Auth"   : [ "client1.der", "client2.der", "client3.der" ],
                    "Server.Auth"   : [ "server1.der", "server2.der" ]
                },

                "MD5" : "f598478b5c316be40c16893bee3c1282"
            },

            "Version"   : "1.9",
            "Signature" : "e161fd8a6ca550425a9173eaf3c8fc1108280f9e"
        }
    )";

    using namespace std;

    mjson::json js(config);
    if (!js.is_valid()) {
        cout << "FAILURE: Format error!" << endl;
        exit(EXIT_FAILURE);
    }

    cout << js["Device"] << ":" << endl;
    cout << " - ID        : " << js["ID"] << "" << endl;
    cout << " - Class     : " << js["Class"] << "" << endl;
    cout << " - Type      : " << js["Type"] << "" << endl;
    cout << " - Version   : " << js["Version"] << "" << endl;
    cout << " - Signature : " << js["Signature"] << "" << endl;

    if (!js.has_object("Firmware")) {
        cout << "FAILURE: missing a mandatory object 'Firmware'!" << endl;
        exit(EXIT_FAILURE);
    }

    mjson::json& frm = js.get_object("Firmware");
    cout << " - Firmware  : " << frm["Version"] << endl;
    cout << "   + MD5    : " << frm["MD5"] << endl;

    if (!frm.has_array("Image")) {
        cout << "FAILURE: missing the 'Image' field!" << endl;
        exit(EXIT_FAILURE);
    }

    const mjson::json::Array& img = frm.get_array("Image");
    cout << "   + Image  : ";
    for (auto& i : img) cout << "[" << i << "], ";
    cout << "\b\b" << " " << endl;

    if (frm.has_object("Update")) {
        mjson::json& upd = frm.get_object("Update");
        cout << "   + Update : [YES]";
        cout << ", URL: '" << upd["Server"] << "'";
        cout << ", Mode: " << upd["Connection"] << endl;
    } else {
        cout << "   + Update : [NO]" << endl;
    }

    exit(EXIT_SUCCESS);
}
