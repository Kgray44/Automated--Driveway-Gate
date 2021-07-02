# USAGE
# python send_plain_message.py --conf config/config.json

# import the necessary packages
from pyimagesearch.notifications import TwilioNotifier
from pyimagesearch.utils import Conf
import argparse

# construct the argument parser and parse the arguments
#ap = argparse.ArgumentParser()
#ap.add_argument("-c", "--conf", required=True,
#    help="Path to the input configuration file")
#args = vars(ap.parse_args())

# load the configuration file and initialize the Twilio notifier
#conf = Conf(args["conf"])
conf = Conf("/home/pi/Desktop/config/sighthound-lpr-cloud-examples/code-samples/python/config.json")
tn = TwilioNotifier(conf)

# send a text message
print("[INFO] sending txt message...")
tn.send("Motion!")
print("[INFO] txt message sent")