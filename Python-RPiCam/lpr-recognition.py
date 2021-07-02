# Filename: lpr-recognition.py
import base64
import http.client
import json
import os
import sys
import cv2
from pyimagesearch.notifications import TwilioNotifier
from pyimagesearch.utils import Conf
import argparse
from picamera import PiCamera
from time import sleep
from Adafruit_IO import Client, Feed, Data
import time
import datetime


ADAFRUIT_IO_KEY = '*****************************'
ADAFRUIT_IO_USERNAME = '***********'
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

camera = PiCamera()

conf = Conf("/home/pi/Desktop/sighthound-lpr-cloud-examples/code-samples/python/config/config.json")
tn = TwilioNotifier(conf)

###############################################################################
##
##  You must edit and replace 'YourSighthoundToken' with your own token.
##
## Your Sighthound Cloud API Token. More information at
##     https://www.sighthound.com/support/creating-api-token
##

_cloud_token = "****************************"

##
###############################################################################

# To draw on test images, a recent version of Pillow is required. 
# Under OS X or Windows install via `pip install Pillow`. 
# Under linux install the `python-imaging` package.
try:
    from PIL import Image, ImageDraw, ImageFont
except:
    print ("")
    print ("########################################################################")
    print ("##")
    print ("##    Cannot draw on images without installing the Pillow package.")
    print ("##        To install Pillow...")
    print ("##")
    print ("##            Under OS-X and Windows, from the command line:")
    print ("##                pip install Pillow")
    print ("##")
    print ("##            Under linux, install the 'python-imaging' package.")
    print ("##")
    print ("########################################################################")
    print ("")
    drawing_allowed = False
else:
    drawing_allowed = True

# Set this variable to True to print all server responses.
_is_verbose = True
## _is_verbose = True

# The cloud server to use, here we set the development server.
_cloud_host = "dev.sighthoundapi.com"

# License Plate Recognition URL
_lpr_url_path = "/v1/recognition?objectType=licenseplate,vehicle"

# The default directory where test images can be found.
_input_folder = "/home/pi/Desktop/sighthound-lpr-cloud-examples/code-samples/python/"

# The default directory where annotated test images will be written.
_output_folder = "out"  # In the current folder


###############################################################################
def verbose(print_line):
    # Used to print out verbose output, if _is_verbose is True
    
    if _is_verbose:
        print (print_line)

        
###############################################################################
def send_request(request_method, request_path, params):
    """A utility function to send API requests to the Sighthound Cloud server.

    This function will abort the script with sys.exit(1) on API errors.
    
    @param  request_method  The request method, "PUT" or "POST".
    @param  request_path    The URL path for the API request.
    @param  params          The parameters of the API request, if any.
    @return response_body   The body of the response.
    """
    # Send the request.
    headers = {"Content-type": "application/json",
               "X-Access-Token": _cloud_token}
    conn = http.client.HTTPSConnection(_cloud_host)
    conn.request(request_method, request_path, params, headers)

    # Process the response.
    response = conn.getresponse()
    body = response.read()
    error = response.status not in [200, 204]

    #if _is_verbose or error:
    print ("")
    print ("Response from URL Request:")
    print ("")
    print (response.status, body)
    print ("")

    if error:
        sys.exit(1)

    return body


###############################################################################
def is_image(filename):
    """A naive utility function to determine images via filename extension.

    @param  filename  The filename to examine.
    @return is_image  True if the file appears to be an image.
    """
    return filename.endswith('.png') or filename.endswith('.jpeg') or \
            filename.endswith('.jpg') or filename.endswith('.bmp')


def do_lpr_reco(input_path, output_path):
    """Send images to test their recognition."""
    print ("")
    print ("Beginning Vehicle Recognition")
    print ("")
    # Create the output directory.
    if not os.path.exists(output_path):
        os.mkdir(output_path)
    
    # Submit all images in the input directory for recognition.
    while True:
        camera.capture('/home/pi/Desktop/sighthound-lpr-cloud-examples/code-samples/python/image.jpg')
        input_file = 'image.jpg'
        verbose ("  Submitting test image ")
        
        ####################################################################
        ##
        ##  This is the Vehicle Recognition Cloud API interface
        ##
        ####################################################################
        
        base64_image = base64.b64encode(open(input_file, "rb").read()).decode("utf-8")
        params = json.dumps({"image": base64_image})
        
        ##  Access the API with a "POST" request that includes the encoded image:
        response = json.loads(send_request("POST", _lpr_url_path, params))

        ####################################################################
        ##  From here on, we are pulling the data from the response
        ####################################################################
        
        if drawing_allowed:
            # Prepare to draw on the image
            image = Image.open(input_file)
            font = ImageFont.load_default
            draw = ImageDraw.Draw(image)

        vehicles = response['objects']  # Each object is a vehicle found in the image
        
        print ("Image file: " + input_file + " - Number of vehicles found: " + str(len(vehicles)))
        
        for vehicle in vehicles:
            annotation = vehicle['vehicleAnnotation']
            if drawing_allowed:
                # Retrieve and draw a bounding box for the detected license plate.
                json_vertices = annotation['bounding']['vertices']
                vert_list = [(point['x'], point['y']) for point in json_vertices]
                draw.polygon(vert_list)
            
            # Retrieve and draw make, model, color, and type with confidences.
            v_system = annotation['attributes']['system']

            v_make_name        = v_system['make']['name']
            v_model_name       = v_system['model']['name']
            v_color_name       = v_system['color']['name']
            
            v_type             = v_system['vehicleType']

            v_make_confidence  = v_system['make']['confidence']
            v_model_confidence = v_system['model']['confidence']
            v_color_confidence = v_system['color']['confidence']

            v_text = "  %s: %s (%s) %s (%s) %s (%s)" % (
                v_type,
                v_make_name,
                v_make_confidence,
                v_model_name,
                v_model_confidence,
                v_color_name,
                v_color_confidence)
                
            if drawing_allowed:
                image_text = "  %s\n    %s (%s)\n    %s (%s)\n    %s (%s)" % (
                    v_type,
                    v_make_name,
                    v_make_confidence,
                    v_model_name,
                    v_model_confidence,
                    v_color_name,
                    v_color_confidence)
                draw.text(vert_list[0], image_text)
                
            try:
                licence_plate = annotation['licenseplate']
            except:
                print (v_text + "  No License Plate Found")
            else:
                if drawing_allowed:
                    # Retrieve and draw a bounding box for the detected license plate.
                    json_vertices = licence_plate['bounding']['vertices']
                    vert_list = [(point['x'], point['y']) for point in json_vertices]
                    draw.polygon(vert_list)

                # Retrieve and draw the region and string with confidences of the recongition.
                lp_region = licence_plate['attributes']['system']['region']
                lp_string = licence_plate['attributes']['system']['string']

                lp_region_name       = lp_region['name']
                lp_string_name       = lp_string['name']

                lp_region_confidence = lp_region['confidence']
                lp_string_confidence = lp_string['confidence']

                lp_text = "  %s (%s) %s (%s)" % (
                    lp_region_name, 
                    lp_region_confidence, 
                    lp_string_name, 
                    lp_string_confidence)
                    
                if drawing_allowed:
                    draw.text(vert_list[0], lp_text)
                print (v_text + "  License Plate Found:" + lp_text)
                
                print("[INFO] sending txt message...")
                tn.send("Car!  Make: " + v_make_name + ", Model: " + v_model_name + ", Color: " + v_color_name + ", Type: " + v_type + ", Region: " + lp_region_name + ", Plate Number: " + lp_string_name)
                print("[INFO] txt message sent")
                
                if lp_string_name == "*******":
                    aio.send('car', 1)
                    tn.send("Car verified. Mom.")
                else:
                    aio.send('car', 0)
                    #tn.send("WARNING etc. Car unverified.")

                if lp_string_name == "*******":
                    aio.send('car', 1)
                    tn.send("Car verified. Dad.")
                    
                else:
                    aio.send('car', 0)
                    #tn.send("WARNING etc. Car unverified.")
                
        if drawing_allowed:
            output_file_path = os.path.join(output_path, input_file)
            print ("")
            print ("Writing annotized image to: " + output_file_path)
            image.save(output_file_path)
            
        print ("")

    verbose ("LPR Reco Complete\n")


###############################################################################
if __name__ == '__main__':
    
    if _cloud_token == "YourSighthoundCloudToken":
        print ("")
        print ("########################################################################")
        print ("##")
        print ("##  Before you can run these examples, you must edit " + sys.argv[0])
        print ("##")
        print ("##  Replace the text 'YourSighthoundToken' near the top of the file.")
        print ("##")
        print ("##  Modify this line:")
        print ("##")
        print ('##      _cloud_token = "YourSighthoundCloudToken"')
        print ("##")
        print ("##  Use your token that you can get from here:")
        print ("##      https://www.sighthound.com/support/creating-api-token")
        print ("##")
        print ("##  More details are in the README.txt file in this directory.")
        print ("##")
        print ("##  Exiting, until this gets resolved.")
        print ("##")
        print ("########################################################################")
        print ("")
        sys.exit(2)
        
    
    if len(sys.argv) > 3:
        print ("Usage: python " + sys.argv[0] + " [path to image directory [path to output directory]]")
        sys.exit(2)

    if len(sys.argv) > 2:
        output_folder = sys.argv[2]
        input_folder = sys.argv[1]
    else:
        output_folder = _output_folder
        if len(sys.argv) > 1:
            input_folder = sys.argv[1]
        else:
            input_folder = _input_folder

    verbose (" input_folder: " + input_folder)
    verbose ("output_folder: " + output_folder)
    verbose ("")

    do_lpr_reco(input_folder, output_folder)
