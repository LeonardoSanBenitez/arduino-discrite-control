# Compile, upload and verify code
# To exit, press Crt-a z x
clear && \
/home/benitez/bin/arduino-cli compile --fqbn "esp8266:esp8266:nodemcuv2" ./embedded_code/ && \
echo "-------------------------------------------------------------------" && \
/home/benitez/bin/arduino-cli upload ./embedded_code/ --port "/dev/ttyUSB0" --fqbn "esp8266:esp8266:nodemcuv2" --verify && \
minicom --baudrate 9600 --device "/dev/ttyUSB0"
