import matplotlib.pyplot as plt

# Lê os dados do ficheiro
with open("times.txt", "r") as f:
    valores = [float(linha.strip()) for linha in f.readlines()]

# Calcula a média
num_amostras = len(valores)
nomes_amostras = [str(i+1) for i in range(num_amostras)]
media = sum(valores) / num_amostras if num_amostras > 0 else 0

# Cria o gráfico
plt.figure(figsize=(12, 8))
plt.bar(nomes_amostras, valores, color='skyblue', label='Valor da Amostra')
plt.axhline(media, color='red', linestyle='--', label=f'Média: {media:.4f}') 

plt.title("Desempenho por número de documentos")
plt.xlabel("Número de Documentos")
plt.ylabel("Tempo (segundos)")
plt.xticks([], [])
plt.grid(axis='y')
plt.legend()
plt.tight_layout()

# Mostra o gráfico
plt.show()

