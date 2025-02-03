# Currentt test project

Develop an I2C communication interface using the ESP-IDF framework (version 5.3) for the 
[ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/index.html). The goal 
is to implement a driver for an I2C-based temperature and humidity sensor and read data 
periodically. The sensor we are using is the [SHT40-AD1B-R2](https://nl.mouser.com/datasheet/2/682/HT_DS_Datasheet_SHT4x-3454169.pdf).
Your program should initialize the I2C interface, configure the sensor, and periodically read and output the 
temperature and humidity data via UART. 


## Key requirements of the project
1. I2C Initialization: 
-  Configure the ESP32-S3 as an I2C master. 
-  Set up the appropriate I2C pins. 
-  Implement error handling for initialization failures. 
2. Sensor Configuration: 
-  Configure the sensor with the appropriate settings. 
-  Verify the sensor is correctly connected by reading its device ID or checking its 
    status register. 
3. Reading Temperature and Humidity Data: 
-  Implement a function to read the data from the sensor. 
-  Convert the raw data to a human-readable format (e.g., Celsius). 
-  Handle edge cases such as communication failures, incorrect data, or sensor 
    errors. 
4. Periodic Data Logging: 
-  Use a FreeRTOS task to read data periodically (e.g., every 2 seconds). 
-  Print the data from the main task to the UART console. 
5. Code Repository 
-  Use Github or Gitlab to publish the code. 
-  Use GIT for version control. 
-  Bonus: Add a CI configuration to build the hex files in Github or Gitlab 
    automatically. 

## Notes from development

- To make compilation and debugging easier, the USB-JTAG, which on the schematic is connected to a possible USB connector. For RS485 data logging, if required to log to the UART0, it would be necessary to implement the protocol to make the data flow to the logging component of the system. So to meet the key requirements, a **REPORT_OVER_CONSOLE**  flag was created that allowed the device to report any other information  (if the console level is set to E/W/I) to the same console output.
- The code was made using a Component for the SHT4x sensor, which handles the initialization, read-out, conversion and eventually the deletion of the device in case of an error to free up memory. It would be required to implement a function that would deal with the failure, generating an output or log the error in a file that can be read by a user/technician


