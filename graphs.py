# from math import log2
# import matplotlib.pyplot as plt

# def f(x, values, last_seen_vals):
#     count = len(last_seen_vals)
#     if int(log2(x + 1)) == count + 1:
#         last_seen_vals.append(x)
#         values.append(count + 1)
#         while count >= 0:
#             last_seen_vals[count - 1] = x
#             count -= 1
#     else:
#         while count > 0:
#             if last_seen_vals[count - 1] + 2 ** (count - 1) == x:
#                 values.append(count)
                
#                 while count > 0:
#                     last_seen_vals[count - 1] = x
#                     count -= 1
#                 break

#             count -= 1
        
# m = 1000000

# lst_of_f_vals = []
# x_vals = [i for i in range(1, m)]
# lst = []
# f_amor = []
# log = []
# sum_of_vals = 0
# for val in x_vals:
#     f(val, lst_of_f_vals, lst)
#     sum_of_vals += lst_of_f_vals[-1]
#     f_amor.append(sum_of_vals / val)
#     log.append(log2(val))

# plt.title("F runtimes")
# plt.plot(lst_of_f_vals, label='F values')
# plt.xlabel('x')
# plt.ylabel('Steps')
# plt.show()

# plt.title("Testing Amortized")
# plt.plot(f_amor, label='F Amortized values')
# # plt.plot(log, label="log values")
# plt.xlabel('x')
# plt.ylabel('Amortized Steps')
# plt.show()

import matplotlib.pyplot as plt
from itertools import product

# Part (a): to_numerical
def all_zeros(value):
    """
    A function that asserts that its values
    are all zero. Returns false otherwise.
    """
    mantissa = value[0]
    exponent = value[1]
    sign = value[2]
    
    for val in mantissa:
        if val != 0:
            return False
    return exponent == 0 and sign == 0

def to_numerical(value):
    """
    A function that takes in a tuple representing a float
    If the float is valid in our floating point system,
    then the number is outputted as a numerical value.
    Otherwise, "Not an FP Number" is outputted.
    
    The floating point system is as follows:
    B = 2
    p = 4
    [L, U] = [-3, 3]

    For this floating point system, we have that [[0, 0, 0, 0], 0, 0]
    represents 0. Everything else is not valid.
    """
    mantissa = value[0]
    exponent = value[1]
    sign = value[2]
    
    # If the mantissa is the right length and starts with 1 (or all zeros), and the exponent 
    # is in the range, and the sign is either 0 or 1
    if len(mantissa) == 4 and (mantissa[0] == 1 or all_zeros(value)) and -3 <= exponent <= 3 and sign in [0, 1]:
        # Valid floating point number in our system 
        retval = sum([mantissa[i] / (2 ** i) for i in range(4)]) * (2 ** exponent)
        if sign == 0 and retval != 0:
            retval *= -1
        return retval
    else:
        # Not a valid FP number
        return "Not an FP Number"

# ==== Test Values ====
print(to_numerical([[0, 0, 0, 0], 0, 0]))
print(to_numerical([[0, 0, 0, 0], 3, 1]))
print(to_numerical([[0, 1, 0, 0], 3, 0]))
print(to_numerical([[1, 0, 1, 1], 2, 1]))

# Part (b): OFL value
upper_bound = to_numerical([[1, 1, 1, 1], 3, 1])
print(upper_bound) # This does equal the value calculated from the formula given in the textbook

# Generate floats function
def generate_floats():
    float_numbers = [to_numerical([[0, 0, 0, 0], 0, 0])]
    mantissas = list(product([0,1], repeat=3))
    exponents = list(range(-3, 4))
    signs = [0, 1]
    for sign in signs:
        for exp in exponents:
            for mantissa in mantissas:
                mantissa = list(mantissa)
                mantissa.insert(0, 1)
                float_numbers.append(to_numerical([mantissa, exp, sign]))
                
    return float_numbers

print(generate_floats())

# # Part (c): relative_error
# def relative_error():