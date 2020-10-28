/*
   Timing Schedule - See readme file
*/

void TXtiming() // Timing
{
  // run additional scripts here to generate data prior to TX if there is a large delay involved.
  if ((timeStatus() == timeSet) && (second() == 0) && (minute() >= 1))
  {
    setGPStime();
    {
    }
    if ((minute() % 10 == 0) && (minute() % 30 != 2) && (second() <= 2)) // TX WSPR standard message
    {
      rf_on();
      loc8calc(); // Get position and update 4-char locator, 6-char locator and last 2 chars of 8-char locator
      call_telem(); // Update WSPR telemetry callsign based on previous information : position and altitude in meters
      loc_dbm_telem(); // Update WSPR telemetry locator and dbm. Get temperature, voltage, speed in knots, GPS status and sats number
      telemetry_set = true;
      //send_serial_data();
      freq = WSPR_FREQ;
      setModeWSPR(); // set WSPR standard mode
      encode(); // begin radio transmission
      rf_off();
    }
    else if ((minute() % 10 == 2) && (second() <= 2) && (telemetry_set == true))    // TX WSPR standard message
    {
      rf_on();
      //send_serial_data();
      freq = WSPR_FREQ;
      setModeWSPR_telem(); // set WSPR standard mode
      encode(); // begin radio transmission
      rf_off();
    }
    else if ((minute() % 10 == 4) && (second() <= 2) && (telemetry_set == true))    // TX WSPR telemetry message
    {
      rf_on();
      //send_serial_data();
      freq = WSPR_FREQ;
      setModeWSPR(); // set WSPR telemetry mode
      encode(); // begin radio transmission
      rf_off();
    }
    else if ((minute() % 10 == 8) && (second() <= 2)) // Check location age/validity and software GPS reset if necessary
    {
      if (gps.location.age() > 600000)
      {
        gps_reset();
        delay(500);
        setGPS_AirBorne(); // set GPS into airborne mode
        delay(500);
        gps_set_max_performance_mode();
        delay(500);
      }
    }
  }
}

