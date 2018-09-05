//
//  main.h
//  esp32-ota-https
//
//  Updating the firmware over the air.
//
//  Created by Andreas Schweizer on 11.01.2017.
//  Copyright © 2017 Classy Code GmbH
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#ifndef __MAIN_H__
#define __MAIN_H__ 1


// Adjust these values for your environment.
// -------------------------------------------------------------------------------------

// Used by the OTA module to check if the current version is different from the version
// on the server, i.e. if an upgrade or downgrade should be performed.
#define SOFTWARE_VERSION          1

// Provide the network name and password of your WIFI network.
#define WIFI_NETWORK_SSID         "SecureUpdate"
#define WIFI_NETWORK_PASSWORD     "EmsecExercise42018"

// Provide server name, path to metadata file and polling interval for OTA updates.
#define OTA_SERVER_HOST_NAME      "www.emsec.cispa.saarland"
#define OTA_SERVER_METADATA_PATH  "/esp32/ota.txt"
#define OTA_POLLING_INTERVAL_S    5
#define OTA_AUTO_REBOOT           1

// Provide the Root CA certificate for chain validation.
// (copied from gd_bundle-g2-g1.crt)
#define OTA_SERVER_ROOT_CA_PEM \
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "-----END CERTIFICATE-----\n"

// Provide the Peer certificate for certificate pinning.
// (copied from classycode.io.crt)
#define OTA_PEER_PEM \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIHETCCBfmgAwIBAgISA1zDvYNSy7ByWrEeLxBWnh3PMA0GCSqGSIb3DQEBCwUA\n" \
    "MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n" \
    "ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xODA2MDQxMzI3MjlaFw0x\n" \
    "ODA5MDIxMzI3MjlaMB8xHTAbBgNVBAMTFGVtc2VjLmNpc3BhLnNhYXJsYW5kMIIC\n" \
    "IjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEArlsHzieqzlOcnUf+iHjS7+PJ\n" \
    "8XlF55j0nyy/jSTGNBVnxsJrNg/qrnM8PKyWUrhX8qPkgvtj/vYzK8/4tIAxy9AM\n" \
    "wDlrYb0ddpO+zy19tpI3Diqv5/ATPrsc3fp/F2WJARepnyhLAXZRHSjLTItmt0Ez\n" \
    "GoSyDrZ+3mK+rJCbbnAHu7gQuun7dXIl6I/gG+/zVXYo/fxnpQEgaLSKC4SXyzNh\n" \
    "mZKp8xBwukA9QDUuRjRCV3a0jWSYlRqFtzTe5+JzcQr/qN0OxltlBO3nk8GdLmVi\n" \
    "z0DrHDH5BrW01dJ/n9GbTJ8TIdAjAfYbel4TafrN26kYyMj8l1J+AdlhcvnXclo9\n" \
    "BNpgKw73F8ccSX2GxREmmHGSmjmnW2GKwq1GfYmGfDaZ9UeKppEuZqW+n1DW5suH\n" \
    "vpgOytLwP6r7XxythAC4QLv6vkN6mymfPhPxqWuoj71j8WJHQpjmIdYj1TWs5nM7\n" \
    "/S0HsGEu7RtFQhLpc2eePlpO7omS729UaBMimXnVkEoif1HCJrE00qH11cU33KrN\n" \
    "MrRzWd7UPBsGdm/c6q/2Vh3EMl0o6U0+UfmN7soYZocBkfK9JcSqEFpgf7X42sdX\n" \
    "WwpFhaKBStf3E8/ksIbyK4lfzyevrflJJQTzHaxTAg7XSSE0nkrRIBz/+ZXx9w1E\n" \
    "cE+b4AcntPq9rgN8448CAwEAAaOCAxowggMWMA4GA1UdDwEB/wQEAwIFoDAdBgNV\n" \
    "HSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4E\n" \
    "FgQUMmUkIczfn4QSg5HjqKXMfibkVScwHwYDVR0jBBgwFoAUqEpqYwR93brm0Tm3\n" \
    "pkVl7/Oo7KEwbwYIKwYBBQUHAQEEYzBhMC4GCCsGAQUFBzABhiJodHRwOi8vb2Nz\n" \
    "cC5pbnQteDMubGV0c2VuY3J5cHQub3JnMC8GCCsGAQUFBzAChiNodHRwOi8vY2Vy\n" \
    "dC5pbnQteDMubGV0c2VuY3J5cHQub3JnLzAfBgNVHREEGDAWghRlbXNlYy5jaXNw\n" \
    "YS5zYWFybGFuZDCB/gYDVR0gBIH2MIHzMAgGBmeBDAECATCB5gYLKwYBBAGC3xMB\n" \
    "AQEwgdYwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3JnMIGr\n" \
    "BggrBgEFBQcCAjCBngyBm1RoaXMgQ2VydGlmaWNhdGUgbWF5IG9ubHkgYmUgcmVs\n" \
    "aWVkIHVwb24gYnkgUmVseWluZyBQYXJ0aWVzIGFuZCBvbmx5IGluIGFjY29yZGFu\n" \
    "Y2Ugd2l0aCB0aGUgQ2VydGlmaWNhdGUgUG9saWN5IGZvdW5kIGF0IGh0dHBzOi8v\n" \
    "bGV0c2VuY3J5cHQub3JnL3JlcG9zaXRvcnkvMIIBAgYKKwYBBAHWeQIEAgSB8wSB\n" \
    "8ADuAHUA23Sv7ssp7LH+yj5xbSzluaq7NveEcYPHXZ1PN7Yfv2QAAAFjyzIfLAAA\n" \
    "BAMARjBEAiBsPxvCP6ZQMyAq3nb4coTL/GaJ0axziTrieI07xaWwFAIgTwZ2Owkr\n" \
    "0Y4RyFMlxNXCANlpaLjwbmRztF4eOAHJc+AAdQApPFGWVMg5ZbqqUPxYB9S3b79Y\n" \
    "eily3KTDDPTlRUf0eAAAAWPLMh8+AAAEAwBGMEQCIFdlrEqn/1h30uNfLTLoIyDO\n" \
    "WsuCbRaMTrEvXocbmduBAiBvrYOLp7GP5cG18BchYg4Qaz15qzfgMKL1vJnoCSYU\n" \
    "rTANBgkqhkiG9w0BAQsFAAOCAQEASeIZziaEiMuGJCdSKT+3paWVr0g3yxKsnXt3\n" \
    "z7yoshi/0FQCKnlG9UXdEs67TKuv4KWTptUajXM88/xsuGrlJd7KMidSsJVZ0pCi\n" \
    "ruJXyxUkVEp/e3/ys4q4knMhXid/4EX9/UY4YR/8lMbqq0owMKPS5WAA3dv944wG\n" \
    "S3k2VvzG3n5wLk9vOYtdixJHpHrof+uCtoAPXyEhnAEH7qr/M8ARuxnBx1ahfsRV\n" \
    "GWeOwZ6a9BFzJcjR7aeYUH0KVCv0pGKv8SmExqTeG9QMx0lOT9RHze+GVpZGN2eI\n" \
    "1y3sVik2aW2+VO/pzlcH/POT4Vq5Q3Ap+arAW4IZ7IlvZMj+7Q==\n" \
    "-----END CERTIFICATE-----\n"

// -------------------------------------------------------------------------------------


#endif // __MAIN_H__
