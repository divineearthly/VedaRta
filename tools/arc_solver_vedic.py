#!/usr/bin/env python3
"""ARC-VEDIC SOLVER — 100-Sutra reasoning for ARC Prize 2026"""

import numpy as np

def sulba_rotate(grid, times=1): return np.rot90(grid, times)
def sulba_flip_h(grid): return np.fliplr(grid)
def sulba_flip_v(grid): return np.flipud(grid)

def pratibha_intuit(inputs, outputs):
    """Try Vedic transformations to find the pattern"""
    if len(inputs) < 1:
        return None, None, 0
    
    ops = [
        ('rotate_90', lambda g: sulba_rotate(g, 1)),
        ('rotate_180', lambda g: sulba_rotate(g, 2)),
        ('rotate_270', lambda g: sulba_rotate(g, 3)),
        ('flip_horizontal', sulba_flip_h),
        ('flip_vertical', sulba_flip_v),
    ]
    
    for op_name, op_fn in ops:
        matches = sum(1 for i in range(len(inputs)) 
                     if np.array_equal(op_fn(inputs[i]), outputs[i]))
        if matches == len(inputs):
            return op_name, None, 0.95
    
    for cfrom in range(10):
        for cto in range(10):
            if cfrom == cto: continue
            matches = sum(1 for i in range(len(inputs))
                         if np.array_equal(np.where(inputs[i]==cfrom, cto, inputs[i]), outputs[i]))
            if matches == len(inputs):
                return 'recolor', (cfrom, cto), 0.85
    
    return None, None, 0

def nyaya_verify(hypothesis, params, inputs, outputs):
    if hypothesis is None: return False
    ops = {'rotate_90': lambda g: sulba_rotate(g,1),
           'rotate_180': lambda g: sulba_rotate(g,2),
           'rotate_270': lambda g: sulba_rotate(g,3),
           'flip_horizontal': sulba_flip_h,
           'flip_vertical': sulba_flip_v}
    for i in range(len(inputs)):
        trial = ops[hypothesis](inputs[i]) if hypothesis in ops else np.where(inputs[i]==params[0], params[1], inputs[i])
        if not np.array_equal(trial, outputs[i]): return False
    return True

def solve_arc_task(train_in, train_out, test_in):
    h, p, c = pratibha_intuit(train_in, train_out)
    if h and nyaya_verify(h, p, train_in, train_out):
        ops = {'rotate_90': lambda g: sulba_rotate(g,1),
               'rotate_180': lambda g: sulba_rotate(g,2),
               'rotate_270': lambda g: sulba_rotate(g,3),
               'flip_horizontal': sulba_flip_h,
               'flip_vertical': sulba_flip_v}
        if h in ops: return ops[h](test_in), h, c
        if h == 'recolor': return np.where(test_in==p[0], p[1], test_in), h, c
    return test_in, None, 0

if __name__ == "__main__":
    print("🕉️  ARC-VEDIC SOLVER")
    tests = [
        ('Rotate 90', [np.array([[1,0],[0,0]])], [np.array([[0,1],[0,0]])], np.array([[1,2],[3,0]])),
        ('Recolor', [np.array([[1,0],[0,1]])], [np.array([[9,0],[0,9]])], np.array([[1,1],[0,1]])),
        ('Flip H', [np.array([[1,2],[3,4]])], [np.array([[2,1],[4,3]])], np.array([[5,6],[7,8]])),
    ]
    s=0
    for name, ti, to, te in tests:
        r, h, c = solve_arc_task(ti, to, te)
        print(f"  {name}: {h} ({c:.0%})")
        if h: s+=1
    print(f"  Solved: {s}/{len(tests)}")
