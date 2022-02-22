import asyncio
import time

from bleak import BleakClient
from datetime import datetime

ADDRESS = "10:52:1C:69:78:76"
SUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

RUNNING = True
TIME_IN_SECONDS = 15
TIME_IN_MINUTES = TIME_IN_SECONDS * 60
ERROR_TIME_IN_SECONDS = 10


async def main():
    start = time.perf_counter() + TIME_IN_MINUTES
    while RUNNING:
        if start <= time.perf_counter():
            start = time.perf_counter() + TIME_IN_MINUTES

            try:
                async with BleakClient(ADDRESS) as client:
                    value = bytes(await client.read_gatt_char(CUUID))

                    now = datetime.now()
                    current_time = now.strftime("%H:%M:%S")
                    print(current_time + ";" + str(value)[2:7:] + "\n")

                    f = open("temperature.txt", "a+")

                    f.write(current_time + ";" + str(value)[2:7:] + "\n")

                    f.close()
            except:
                start = time.perf_counter() + ERROR_TIME_IN_SECONDS
                print("An exception occurred")


asyncio.run(main())

'''
async with BleakClient(ADDRESS) as client:
    value = bytes(await client.read_gatt_char(CUUID))
    print(value)
    
    '''
