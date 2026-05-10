def generate_sattvic_data():
    print("🧘 [Viveka] Creating RAW text training buffer...")
    # We use a very clean, simple text format to avoid the buffer overflow
    with open("cleaned_data.txt", "w") as f:
        for i in range(50):
            f.write(f"Vedic Math: To multiply by 9, use the Ekadhikena Purvena sutra. {i}\n")
    print("✅ [Viveka] Raw buffer prepared.")

if __name__ == "__main__":
    generate_sattvic_data()
