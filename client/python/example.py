import os
import os.path as osp
import base64
import requests
import pprint

import cv2


def image_to_base64_string(image):
    _, buffer = cv2.imencode('.jpeg', image)
    image_string = base64.b64encode(buffer).decode("utf-8") 
    
    return image_string

def send_request(url, data):
    resp = requests.post(url, json=data)
    
    return resp.json()
    
def main():
    URL = 'http://127.0.0.1:30002'
    
    current_dir = osp.dirname(osp.realpath(__file__))
    image_dir = os.path.abspath(os.path.join(current_dir, os.pardir))
    image_file = osp.join(image_dir, 'test-image.jpeg')
    image = cv2.imread(image_file)
    image_string = image_to_base64_string(image)
    
    resp = send_request(URL, {'data': image_string})
    pprint.pprint(resp)
    
if __name__ == '__main__':
    main()