#Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
import json
import base64

def decode_sender_fingerprint(json_data):
    # Parse the JSON data
    parsed_json = json.loads(json_data)
    
    # Extract the Base64 encoded sender fingerprint
    sender_fingerprint_base64 = parsed_json["data"]["sender"]
    
    # Decode the Base64 encoded fingerprint
    try:
        decoded_fingerprint = base64.b64decode(sender_fingerprint_base64).decode('utf-8')
    except (base64.binascii.Error, UnicodeDecodeError) as e:
        return f"Error decoding fingerprint: {e}"
    
    return decoded_fingerprint
