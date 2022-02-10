Brief :
This task is about to create two applications communicating with each other (high-level IPC) where application A
monitors the changes to a file which contains surrounding Wireless APs in JSON format and informs application B
which is responsible for displaying the change in format of:

- SSID’s SNR and/or channel value has changed from X to Y
- SSID is added to the list with SNR <SNR> and channel <CHANNEL>
- SSID is removed from the list.

cat /tmp/access_points (after X seconds)

```json
{
  "access_points": [
    {
      "ssid": "MyAP",
      "snr": 82,
      "channel": 11
    },
    {
      "ssid": "YourAP",
      "snr": 42,
      "channel": 6
    },
    {
      "ssid": "HerAP",
      "snr": 71,
      "channel": 1
    }
  ]
}
```

Expected output:

- MyAP’s SNR has changed from 63 to 82
- YourAP’s channel has changed from 1 to 6
- HisAP is removed from the list
- HerAP is added to the list with SNR 71 and channel 1

How To:
1- Put your json file named "access_points" under /tmp/
2- Open a terminal and run the follewing commands :
   git clone https://github.com/mouertani/code_se.git
   cd code_se
   cmake . && make
   ./Server/bin/SocketServer
   ./Client/bin/SocketClient
3- Modify /tmp/access_points file by adding, removing, modifying and check the response from server side.

Execution environment: Linux (18.04.6 LTS (Bionic Beaver))

![code_se](https://user-images.githubusercontent.com/98718335/152874544-3d2d20ce-8f90-489c-bd55-b94fa9e0de49.png)
