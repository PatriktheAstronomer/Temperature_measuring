# Temperature_measuring
Temperature and humidity measurement station (thermistor and SHT 75 or DHT 22) sending data via ethernet to MySQL (and then to Grafana)<br />
This project contains:<br />
- Circuits diagram (with RTC, which I do not use, because MySQL has timestamp ability, for offline purposes I deeply recommend it!) [jpg/fzz]<br />
- Arduino code<br />
- DHT 22 odd code<br />
- php code<br />
- Python script to sending data from Comet 1140 (represented by website), then we get XML file a its values we send to the database, too<br />
- MySQL/Grafana example code to create a diagram<br />
