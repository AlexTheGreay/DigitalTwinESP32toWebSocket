# Import WebSocket client library
# In order to import, uninstall all websocket and websockets libraries, and reinstall websocket-client.
from websocket import create_connection

# Connect to WebSocket API and subscribe to trade feed for XBT/USD and XRP/USD
ws = create_connection("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod", header = {'x-api-key':'zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8'})
ws.send('{"event":"subscribe", "subscription":{"name":"trade"}, "pair":["XBT/USD","XRP/USD"]}')

# Infinite loop waiting for WebSocket data
while True:
    print(ws.recv())
    ws.send("SentString");

