from geopy.distance import geodesic

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


