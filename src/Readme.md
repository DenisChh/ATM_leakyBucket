# ATM Leaky Bucket

A modest software simulating data trafic between clients (here known as sources) and a server.
The packets sent by the client are queued into a hypothetical "bucket" before beeing actually sent to the network
at a constant rate (hence the leaky bucket). If data doesn't leave the bucket fast enough, eventually the bucket
will start to overflow and data packets will be lost.

## Installation
* Go to src folder and use make install if you don't have GTK library installed yet (sudo permissions needed)
```bash
cd src
make install
```

* Use make run to compile and execute the software
```bash
make run
```

## Usage
* To create a new simulation, use the Nouvelle Simulation button
* Choose a folder to save your simulation data in
* Specify the rate at which the data will leave your bucket (packets/second)
* Specify the maximum number of packets your bucket can hold
* Specify the total number of tokens available in your bucket. Each packet will be attributed a token before 
leaving the bucket, so make sure you have enough available or you might experience data loss. The tokens are
then retrieved once the packet left your bucket.
* Add sources by using the Sources button. Specify the rate at which the client will send its packets and an its
initial state (turn it on to make the client send data packets at the start of the simulation)
* Start the simulation by using the Lancer simulation button
* Stop the simulation by using the Stopper simulation button
* Use sauvegarder/charger simulation buttons if you wish to save your simulation or load a previously saved one


