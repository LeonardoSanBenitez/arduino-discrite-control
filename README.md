# esp8266-discrite-control
Implementation of a discrite control system for ESP8266



Compile and upload code:
```
/home/benitez/bin/arduino-cli compile --fqbn "esp8266:esp8266:nodemcuv2" ./embedded_code/ && \
echo "-------------------------------------------------------------------" && \
/home/benitez/bin/arduino-cli upload ./embedded_code/ --port "/dev/ttyUSB0" --fqbn "esp8266:esp8266:nodemcuv2" --verify

```

