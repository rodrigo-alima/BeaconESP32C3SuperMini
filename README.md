# Modo de Utilização do Beacon

## Passo a Passo

1. **Transferência do Programa**  
   Após transferir o programa ao microcontrolador, no primeiro uso ele não terá mensagens gravadas.

2. **Configuração da Rede Wi-Fi**  
   Certifique-se de que há um celular compartilhando a rede Wi-Fi com os seguintes valores (definidos no software):  
   - **SSID:** `BEACON`  
   - **Senha:** `12345678`

3. **Conexão e Obtenção do IP**  
   Quando conectado, o dispositivo exibirá no display o **IP** atribuído. 

4. **Envio de Mensagem**  
   Utilize qualquer programa que envie mensagens via **UDP**:  
   - Configure o **IP** exibido pelo dispositivo.  
   - Use a **porta padrão**: `55000` (que pode ser modificada no software, se necessário).

5. **Início da Transmissão**  
   Após o dispositivo receber a mensagem:  
   - O módulo Wi-Fi será desligado.  
   - O display será desativado.  
   - A transmissão da mensagem será iniciada.
