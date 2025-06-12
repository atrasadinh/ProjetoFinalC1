Este é um sistema simples de gerenciamento de pacientes e agendamentos desenvolvido em C/C++. Ele permite o cadastro, exclusão e listagem de pacientes, além do agendamento e cancelamento de consultas, com validações básicas de dados

Funcionalidades
Cadastro de Pacientes:
Registra pacientes com CPF, nome, data de nascimento e calcula a idade.
Validação de CPF (11 dígitos numéricos, não pode ser sequência de dígitos iguais).
Validação de nome (mínimo 4 caracteres).
Validação de data de nascimento (DDMMAAAA, paciente deve ter no mínimo 13 anos).
Verificação se o CPF já está cadastrado.
Exclusão de Pacientes:
Permite excluir um paciente pelo CPF.
Impede a exclusão de pacientes com agendamentos futuros.
Remove agendamentos passados do paciente ao ser excluído.
Agendamento de Consultas:
Agendamento de consultas para pacientes existentes.
Validação de data da consulta (futura, no formato DDMMAAAA).
Validação de horário da consulta (HHMM, com minutos em 00, 15, 30, 45).
Verificação de sobreposição de horários.
Horário de funcionamento do consultório (08:00 às 19:00).
Verificação se o paciente já possui um agendamento futuro.
Cancelamento de Agendamentos:
Permite cancelar um agendamento futuro de um paciente pelo CPF.
Listagem de Pacientes:
Exibe todos os pacientes cadastrados.
Opção de ordenação por CPF ou por nome.
Mostra o próximo agendamento do paciente (se houver).
Listagem da Agenda:
Permite listar todos os agendamentos ou filtrar por um período específico.
Os agendamentos são exibidos ordenados por data e hora.
