// kenwood_pc.h
/*
Flex Fork created by Michael Walker VA3MW - 09/2017
Tested on a Flex 6300
Connection to radio via FTDI USB adapter directly connected to the 6000 series radio
   details on Flex radio configuration available from Flex Radio website
Controller must be configured to request mode to request frequency data

Note:  Band 0 will be selected if freq falls out of range.  Adjust your band limits accordingly

To do:  Ensure band controller is following active TX slice
*/

while (Serial.available()) {
    rdKS="";
    Serial.readBytesUntil(lf, rdK, 14);       // fill array from serial 'lf' is defined in main code
        if (rdK[0] == 70 && rdK[1] == 66){     // filter
            for (int i=2; i<=12; i++){          // 3-13 position to freq
                rdKS = rdKS + String(rdK[i]);   // append variable to string
            }
            freq = rdKS.toInt();

            //=====[ Frequency (Hz) to Band rules ]======================================
            //                                        you can expand rules up to 14 Bands

                 if (freq >=    400000 && freq <=   2000000 )  {BAND=1;}  // 160m
            else if (freq >=   2000001 && freq <=   3650000 )  {BAND=2;}  //  80m - cw
            else if (freq >=   3650001 && freq <=   6000000 )  {BAND=3;}  //  80m - ssb
            else if (freq >=   6000001 && freq <=   9500000 )  {BAND=4;}  //  40m
            else if (freq >=   9500001 && freq <=  13000000 )  {BAND=5;}  //  30m
            else if (freq >=  13000001 && freq <=  17000000 )  {BAND=6;}  //  20m
            else if (freq >=  17000001 && freq <=  19000000 )  {BAND=7;}  //  17m
            else if (freq >=  19000001 && freq <=  24000000 )  {BAND=8;}  //  15m
            else if (freq >=  24000001 && freq <=  27000000 )  {BAND=9;}  //  12m
            else if (freq >=  27000001 && freq <=  40000000 )  {BAND=10;}  //  10m
            else if (freq >=  40000001 && freq <=  60000000 )  {BAND=12;}  //   6m
            else if (freq >= 144000000 && freq <= 146000000 )  {BAND=13;}  //   2m
            else {BAND=0;}                                                // out of range
            //===========================================================================

            bandSET();                                              // set outputs relay

            #if defined(REMOTE_RELAY)
                remoteRelay();
                previous3 = millis();     // set time mark
            #endif
            #if defined(SERIAL_echo)
                serialEcho();
            #endif
            #if defined(WATCHDOG)
                previous = millis();                   // set time mark
            #endif
        }
        memset(rdK, 0, sizeof(rdK));   // Clear contents of Buffer
}

#if defined(WATCHDOG)
    watchDog();
#endif

#if defined(REQUEST)
    timeout2 = millis()-previous2;                  // check timeout
    if (timeout2>(watchdog2)){
        Serial.print("IF;");
        Serial.flush();
//        CATdata = "";
        previous2 = millis();                       // set time mark
    }
#endif
