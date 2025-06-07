import numpy as np
import faiss  # make sure faiss is installed: pip install faiss-cpu

# Step 1: Create dummy embeddings (e.g., 1000 vectors of dimension 128)
dimension = 128
num_vectors = 1000
np.random.seed(42)
embeddings = np.random.random((num_vectors, dimension)).astype('float32')

# Step 2: Build the ANN index (using IndexFlatL2 for simplicity - exact search)
index = faiss.IndexFlatL2(dimension)  # L2 distance metric
index.add(embeddings)  # Add all embeddings to the index

# Step 3: Create a query vector (random or embedded query chunk)
query_vector = np.random.random((1, dimension)).astype('float32')

# Step 4: Search for the top 5 nearest neighbors
k = 5
distances, indices = index.search(query_vector, k)

print("Top 5 nearest neighbors indices:", indices)
print("Distances:", distances)
