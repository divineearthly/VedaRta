# VedaRta Language Specification v1.0

## Etymology
Veda = Knowledge, Vedic science, mathematics
Rta = Cosmic order, Dharma, truth

## File Extension
.vr

## Grammar (EBNF)
program = { kosha_block | statement }
kosha_block = kosha_name "{" { statement } "}"
kosha_name = "annamaya" | "pranamaya" | "manomaya" | "vijnanamaya" | "anandamaya"
statement = assignment | func_call | output_stmt
assignment = IDENT "=" expression
expression = array_literal | func_call | IDENT | NUMBER
array_literal = "[" NUMBER { "," NUMBER } "]"
func_call = IDENT "(" [ expression { "," expression } ] ")"
output_stmt = "output" "(" expression ")"

## Keywords
| Keyword | Sanskrit | Function |
|---------|----------|----------|
| ekadhikena | एकाधिकेन | x>=0?1.05x:0.3x |
| trinadi | त्रिनाडी | Sushumna+Ida+Pingala blend |
| shunyam | शून्यम् | Zero-mean normalization |
| sphota | स्फोट | O(n) attention |
| urdhva_tiryag | ऊर्ध्वतिर्यग् | 4x4 NEON matmul |
| nikhilam | निखिलम् | Complement arithmetic |
| chandra_kala | चन्द्रकला | Cosine LR schedule |
| bija_momentum | बीज | PHI-based momentum |
| karma_phala | कर्मफल | MSE loss |

## Compilation Pipeline
.vr → Lexer → Parser → C++ Transpiler → g++ ARM64 NEON → Binary

## Type System
All values are float32. No garbage collection. Vedic memory model (Pancha Kosha).

## Runtime
ARM64, 4GB RAM, no GPU, no cloud. Android/Termux or Linux.
