# Abrir o ficheiro original e ler as linhas
with open('vuln.txt', 'r') as f:
    linhas = f.readlines()

# Abrir um novo ficheiro para guardar o resultado
with open('formated.txt', 'w') as f:
    for linha in linhas:
        linha = linha.strip()
        if not linha:
            continue
        try:
            ip_porta, user_pass = linha.split()
            ip, _ = ip_porta.split(':')
            user, password = user_pass.split(':')
            nova_linha = f'{ip}:{user}:{password}\n'
            f.write(nova_linha)
        except ValueError:
            print(f'Linha mal formatada: {linha}')
