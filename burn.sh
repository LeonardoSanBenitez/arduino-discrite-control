# Compile, upload and verify code
# To exit, press Crt-a z x
clear && \
/home/benitez/bin/arduino-cli compile --fqbn "arduino:avr:mega" ./embedded_code/ && \
echo "-------------------------------------------------------------------" && \
/home/benitez/bin/arduino-cli upload ./embedded_code/ --port "/dev/ttyACM0" --fqbn "arduino:avr:mega" --verify && \
/home/benitez/bin/arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200
