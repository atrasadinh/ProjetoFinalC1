# Sistema de Gerenciamento de Pacientes e Agendamentos

Este é um sistema simples de gerenciamento de pacientes e agendamentos desenvolvido em **C/C++**.  
Ele permite o **cadastro**, **exclusão** e **listagem de pacientes**, além do **agendamento** e **cancelamento de consultas**, com validações básicas de dados.

## ✨ Funcionalidades

### 👤 Cadastro de Pacientes
- Registra pacientes com **CPF, nome, data de nascimento** e calcula a idade.
- Validações:
  - CPF: 11 dígitos numéricos, não pode conter todos os dígitos iguais.
  - Nome: mínimo de 4 caracteres.
  - Data de nascimento: formato `DDMMAAAA`, idade mínima de 13 anos.
- Verifica se o CPF já está cadastrado.

### 🗑️ Exclusão de Pacientes
- Exclusão por CPF.
- Impede a exclusão de pacientes com agendamentos futuros.
- Remove agendamentos passados do paciente excluído.

### 📅 Agendamento de Consultas
- Permite agendar consultas apenas para pacientes já cadastrados.
- Validações:
  - Data futura no formato `DDMMAAAA`.
  - Horário no formato `HHMM` (minutos devem ser 00, 15, 30 ou 45).
  - Horário de funcionamento: **08:00 às 19:00**.
  - Verifica:
    - Se o paciente já possui um agendamento futuro.
    - Se há sobreposição com outro agendamento.

### ❌ Cancelamento de Agendamentos
- Cancela agendamentos futuros pelo CPF do paciente.

### 📃 Listagem de Pacientes
- Exibe todos os pacientes cadastrados.
- Ordenação por **CPF** ou **nome**.
- Mostra o **próximo agendamento** (se houver).

### 📖 Listagem da Agenda
- Lista todos os agendamentos ou filtra por um **período específico**.
- Resultados ordenados por **data e hora**.
