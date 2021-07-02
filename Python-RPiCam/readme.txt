Sighthound Cloud Vehicle License Plate Recognition API Example - Python 2.7.11+

Verify python on your computer

    Open a command line

    Check you python version with the following command:
        python --version

    If you get an error like "python: command not found" then you will need to install python.
        Find it here: https://www.python.org/downloads/

    If your python version is less than 2.7.11, then you will need to upgrade it.

Install the Pillow package

    Under OS-X and Windows, from the command line:
        pip install Pillow
    
    Under linux, install the 'python-imaging' package.

Set your Sighthound Cloud Token

    In your browser, go to: https://www.sighthound.com/support/creating-api-token
    You might need to create an account, if you don't already have one.
    Or, press the "Sighthound Cloud section" if you do have an account.
    If you have a token under "Your API Tokens" then use that token.
    Or, press the "New Token" button to generate your token and use that one.
    Your token is identified by a string of random looking letters and numbers.
    
    Make sure you are in "sighthound-lpr-cloud-examples/code-samples-python" for the following:
    
    Edit the file "lpr-recognition.py" and find this line:
        "_cloud_token = "YourSighthoundCloudToken"
    Replace "YourSighthoundCloudToken" with your token string.
    Save the file and return to the command line.
    
    From the command line, in the python directory, perform the following command:
    
        python lpr-recognition.py
    
    when it completed, there will be a new directory in the python directory, called "out".
    This is where the test images are rewritten with annotations for cars and license plates.

Supporting Files

    The image files used for this example are in "sighthound-lpr-cloud-examples/images"
    
    
    
    
NOTICE: invalid library of httplib used which is outdated; only works with Python 2.7.  Update to http.client.
    
