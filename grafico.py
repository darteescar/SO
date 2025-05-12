import matplotlib.pyplot as plt

# Lê os dados do ficheiro
with open("times.txt", "r") as f:
    valores = [float(linha.strip()) for linha in f.readlines()]

# Cria os eixos X como índices (1 a N)
x = list(range(1, len(valores) + 1))

# Cria o gráfico
plt.figure(figsize=(10, 6))
plt.plot(x, valores, marker='o', linestyle='None', color='blue')
plt.title("Desempenho por Adição de Documentos")
plt.xlabel("Número de Documentos")
plt.ylabel("Tempo (segundos)")
plt.grid(True)
plt.tight_layout()

# Mostra o gráfico
plt.show()
