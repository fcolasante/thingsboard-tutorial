OPEN_WEATHER_TOKEN = '7fc6a1ef81de8b2c08260ed791b45032'
DEVICE_1_TOKEN = "A1_TEST_TOKEN"
DEVICE_2_TOKEN = "A2_TEST_TOKEN"
DEVICE_3_TOKEN = "A3_TEST_TOKEN"


def getOpenWeatherToken():
    return OPEN_WEATHER_TOKEN

def getDeviceToken(id):
    return DEVICE_3_TOKEN


CITIES = [['V12mMG95NmrJ8Wu4vfB1', 'Chieti'],
            ['TldN65VTzMukZnAWcKaq', 'Francavilla al Mare'],
            ['ssWgp3cschyZJf37rLzp', 'L\'Aquila'],
            ['qZdNlOVhuMKf7fRU5RAA', 'Teramo']
        ]

NUM_CITIES = len(CITIES)
def getCities(id):
    return CITIES[id]