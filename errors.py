class ValidationError(Exception):
    """Base class for client-side validation errors.

    This is a simple domain exception used by input validation. Transport
    (HTTP) concerns are handled at the API boundary (`main.py`).
    """
    status_code = 400

    def __init__(self, message: str):
        super().__init__(message)


class MissingFieldError(ValidationError):
    pass


class InvalidTypeError(ValidationError):
    pass


class MatrixShapeError(ValidationError):
    pass


class MatrixValueError(ValidationError):
    pass


class ProfitsLengthError(ValidationError):
    pass


class OutOfBoundsError(ValidationError):
    pass


class NegativeValueError(ValidationError):
    pass


class InternalConversionError(Exception):
    """Server-side error during internal conversion to C++ DTOs or similar."""
    status_code = 500
