#!/bin/bash -l

## Nazwa zlecenia
#SBATCH -J MPITest1SBATCH1

## Liczba alokowanych węzłów
#SBATCH -N 32

## Liczba zadań per węzeł (domyślnie jest to liczba alokowanych rdzeni na węźle)
#SBATCH --ntasks-per-node=24

## Ilość pamięci przypadającej na jeden rdzeń obliczeniowy (domyślnie 5GB na rdzeń)
#SBATCH --mem-per-cpu=5GB

## Maksymalny czas trwania zlecenia (format HH:MM:SS)
#SBATCH --time=00:10:00

## Nazwa grantu do rozliczenia zużycia zasobów
#SBATCH -A plgcpu

## Plik ze standardowym wyjściem
#SBATCH --output="output.out"

## Plik ze standardowym wyjściem błędów
#SBATCH --error="error.err"

module add plgrid/tools/openmpi

mpiexec ./ChessEngineProjectMPI ChessEngineProjectConfig2.xml