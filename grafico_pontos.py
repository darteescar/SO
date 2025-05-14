import matplotlib.pyplot as plt

# Lê os dados do ficheiro
with open("times.txt", "r") as f:
    valores = [float(linha.strip()) for linha in f.readlines()]

# Cria os eixos X como índices (1 a N)
x = list(range(1, len(valores) + 1))

# Calcula a média
media = sum(valores) / len(valores) if len(valores) > 0 else 0

# Cria o gráfico
plt.figure(figsize=(10, 6))
plt.plot(x, valores, marker='o', linestyle='None', color='blue')
plt.axhline(media, color='red', linestyle='--', label='Média')

# Adiciona a caixinha com o valor da média
plt.text(
    0.95, 0.95, f'Média: {media:.6f}', transform=plt.gca().transAxes,
    va='top', ha='right',
    bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.3')
)


plt.title("Desempenho por número de documentos")
plt.xlabel("Número de Documentos")
plt.ylabel("Tempo (segundos)")
plt.grid(True)
plt.tight_layout()

# Mostra o gráfico
plt.show()
