from geopy.distance import geodesic
import requests
import os
from dotenv import load_dotenv


def construct_geodesic_adjacency_matrix(locations):
    n = len(locations)
    adjacency_matrix = [[0.0 for _ in range(n)] for _ in range(n)]
    
    for i in range(n):
        for j in range(n):
            if i != j:
                point1 = (locations[i].lat, locations[i].lon)
                point2 = (locations[j].lat, locations[j].lon)
                distance = geodesic(point1, point2).kilometers
                adjacency_matrix[i][j] = distance
            else:
                adjacency_matrix[i][j] = 0.0
                
    return adjacency_matrix


def construct_osr_adjacency_matrix(locations):
    body = {"locations": [[loc.lon, loc.lat] for loc in locations],
            "metrics": ["duration"]}

    load_dotenv()
    headers = {
        'Accept': 'application/json, application/geo+json, application/gpx+xml, img/png; charset=utf-8',
        'Authorization': os.getenv('ORS_API_KEY'),
        'Content-Type': 'application/json; charset=utf-8'
    }
    call = requests.post('https://api.openrouteservice.org/v2/matrix/driving-car', json=body, headers=headers)

    if call.status_code != 200:
        raise Exception(f"Error fetching OSR matrix: {call.status_code} {call.reason} {call.text}")

    data = call.json()
    return data['durations']