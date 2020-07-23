#include "mjson/mjson.hpp"

int main() {

#if 0
    const auto j2 = R"(
        {
            "url"                   : "lkpcoh3a.sps-stg.seacert.com/sps/Personalization",
            "namespace"             : ns,
            "company"               : companyc,
            "personality"           : Controller,
            "class"                 : seacert,
            "business.token"        : "000702030405060708",
            "fingerprint"           : testFingerprint,
            "bootstrap.enc.private" : "/CompanyC-Stg-BSK/companyc_enc_13BSK-enc1T.p8.skb",
            "bootstrap.enc.public"  : "/CompanyC-Stg-BSK/companyc_enc_13BSK-enc1T.der",
            "bootstrap.sign.private": "/CompanyC-Stg-BSK/sig1T.p8.skb",
            "bootstrap.sign.chain"  : [ "/CompanyC-Stg-BSK/sig1T.der" ],
            "client.auth.chain"     : [ "/CompanyC-Stg-BSK/sig1T.der", "Seacert_Intermidiate.der" ],
            "server.auth.chain"     : [ "Seacert_Service_TA.der" ]
        }
    )";
#else
    const auto j2 = R"(
        {
            "object" : {
                "key1" : value1,
                "key2" : value2,
                "key3" : value3,
                "array1" : [ "val1", "val2", "val3", "val4" ]
            }
        }
    )";
#endif

    using namespace std;

    mjson::json js(j2);

    if (!js.is_valid()) {
        cout << "FAILURE: Format error!" << endl;
        return 1;
    }

    if (js.has_object("object")) {
        mjson::json& obj = js.get_object("object");
        cout << "key1   : " << obj["key1"] << endl;
        cout << "key2   : " << obj["key2"] << endl;
        cout << "key3   : " << obj["key3"] << endl;
        cout << "array1 : sz = " << obj.get_array("array1").size() << endl;
    }

    cout << "url                   : " << js["url"] << endl;
    cout << "namespace             : " << js["namespace"] << endl;
    cout << "company               : " << js["company"] << endl;
    cout << "personality           : " << js["personality"] << endl;
    cout << "class                 : " << js["class"] << endl;
    cout << "business.token        : " << js["business.token"] << endl;
    cout << "fingerprint           : " << js["fingerprint"] << endl;
    cout << "bootstrap.enc.private : " << js["bootstrap.enc.private"] << endl;
    cout << "bootstrap.enc.public  : " << js["bootstrap.enc.public"] << endl;
    cout << "bootstrap.sign.private: " << js["bootstrap.sign.private"] << endl;
    cout << "bootstrap.sign.chain  : " << js.get_array("bootstrap.sign.chain").size() << endl;
    cout << "client.auth.chain     : " << js.get_array("client.auth.chain").size() << endl;
    cout << "server.auth.chain     : " << js.get_array("server.auth.chain").size() << endl;

    return 0;
}
