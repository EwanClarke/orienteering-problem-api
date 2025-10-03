from fastapi.testclient import TestClient
import main
import models


def test_solve_happy_path(monkeypatch):
    client = TestClient(main.app)

    # stub the solver to return a simple path
    def fake_solver(input_data):
        # The application now converts input to a DTO-like object (pybind11 struct).
        # Just assert it has the adjacency_matrix attribute so the test stays robust.
        assert hasattr(input_data, "adjacency_matrix")
        # Return indices within the provided 3-node matrix
        return [0, 1, 2]

    # monkeypatch the module-level mapping to use our fake solver
    monkeypatch.setitem(models.FUNCTION_DICT, 'nearest_neighbour', fake_solver)

    payload = {
        "adjacency_matrix": [[0.0, 1.0, 2.0], [1.0, 0.0, 1.0], [2.0, 1.0, 0.0]],
        "profits": [0.0, 10.0, 5.0],
        "budget": 10.0,
        "start_node": 0,
        "end_node": 2,
        "algorithm": "nearest_neighbour"
    }

    r = client.post("/solve", json=payload)
    assert r.status_code == 200
    body = r.json()
    assert body["path"] == [0, 1, 2]


def test_solve_missing_keys(monkeypatch):
    client = TestClient(main.app)

    # stub solver
    def fake_solver(input_data):
        return [0, 1, 2]

    monkeypatch.setitem(models.FUNCTION_DICT, 'nearest_neighbour', fake_solver)

    # remove profits key to trigger validation
    payload = {
        "adjacency_matrix": [[0.0, 1.0], [1.0, 0.0]],
        # "profits": [0.0, 1.0],
        "budget": 10.0,
        "start_node": 0,
        "end_node": 1,
        "algorithm": "nearest_neighbour"
    }

    r = client.post("/solve", json=payload)
    assert r.status_code == 400
