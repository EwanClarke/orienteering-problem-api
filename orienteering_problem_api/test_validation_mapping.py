from fastapi.testclient import TestClient
import main
import models


def test_validation_error_returns_400(monkeypatch):
    client = TestClient(main.app)

    def fake_solver(input_data):
        return [0, 1]

    monkeypatch.setitem(models.FUNCTION_DICT, 'nearest_neighbour', fake_solver)

    payload = {
        "adjacency_matrix": [[0.0, 1.0], [1.0, 0.0]],
        # missing profits -> should trigger ValidationError in validate_input
        "budget": 10.0,
        "start_node": 0,
        "end_node": 1,
        "algorithm": "nearest_neighbour"
    }

    r = client.post("/solve", json=payload)
    assert r.status_code == 400


def test_internal_conversion_error_returns_500(monkeypatch):
    """Simulate an internal conversion error by making InputData.to_solver_input return
    an object missing expected attributes (so input_validation raises InternalConversionError).
    """
    client = TestClient(main.app)

    def fake_solver(input_data):
        return [0, 1]

    monkeypatch.setitem(models.FUNCTION_DICT, 'nearest_neighbour', fake_solver)

    class BrokenDTO:
        # missing adjacency_matrix, profits, etc.
        pass

    # monkeypatch the to_solver_input method to return a broken object
    monkeypatch.setattr(models.InputData, 'to_solver_input', lambda self: BrokenDTO())

    payload = {
        "adjacency_matrix": [[0.0, 1.0], [1.0, 0.0]],
        "profits": [0.0, 1.0],
        "budget": 10.0,
        "start_node": 0,
        "end_node": 1,
        "algorithm": "nearest_neighbour"
    }

    r = client.post("/solve", json=payload)
    assert r.status_code == 500
