"""
Bourgeois Noé
"""
import sys
import time
# Import des fonctions et types
from ctypes import CDLL, c_char_p

"""Les codes 65 à 90 représentent les majuscules.
Les codes 97 à 122 représentent les minuscules."""


def uncaesar(msg, right_shift, shift):
    """
    prend un message, chiffré, et l’information de décalage en paramètre
    :param shift: int : nombre de decalages a appliquer sur chaque char
    :param right_shift: bool
    :param msg: str: alphabetical maj or space suite
    :return: le message en clair
    """
    uncaesared = ''
    for char in msg:
        if 64 < ord(char) < 91:
            if right_shift:
                if 90 - ord(char) <= shift:
                    uncaesared += chr(64 + (shift - (90 - ord(char))))
                else:
                    uncaesared += chr(ord(char) + shift)
            else:
                if ord(char) - 64 <= shift:
                    uncaesared += chr(90 - (shift - (ord(char) -
                                                     64)))  # duplication
                else:
                    uncaesared += chr(ord(char) - shift)  # duplication
        elif ord(char) == 32:
            uncaesared += char
    return uncaesared


class Decrypter:
    def __init__(self):
        self.filename: str = sys.argv[3]


class Asterix(Decrypter):
    def __init__(self):
        super().__init__()
        self.right_shift: bool = sys.argv[1] == "D"
        self.shift: int = int(sys.argv[2])

    def decrypt(self):
        print(
            f"Communication chiffrée avec César {sys.argv[1]} "
            f"{self.shift} "
            f"via {self.filename}")
        with open(self.filename) as f:
            user_input = None
            while not user_input:
                line = f.readline()
                if line:
                    print(f"Nouveau message: {line} "
                          f"-> "
                          f""
                          f"{uncaesar(line, self.right_shift, self.shift)}")
                    user_input = input("Press Enter to continue or "
                                       "enter \"end\" here: ")
                time.sleep(1)


class AlanTuring(Decrypter):
    def __init__(self):
        super().__init__()

    def decrypt(self):
        print(
            f"Communication chiffrée avec Enigma via "
            f"{self.filename}")
        with open(self.filename) as f:
            user_input = None
            enigmalib = CDLL("./enigma.so")
            # On definit le type de retour de la fonction enigma. c_char_p indique
            # que le type de retour est char∗ (c_char_p = C char pointer)
            enigmalib.enigma.restype = c_char_p
            # On indique le type du seul argument, soit c_char_p. Notez que
            # nous assignons une liste, sachant qu’une fonction peut avoir
            # plusieurs arguments.
            enigmalib.enigma.argstype = [c_char_p]
            while not user_input:
                line = f.readline()
                print(line)
                line = line.strip()
                if line:
                    message = ''
                    for i in range(len(line)):
                        char = line[i]
                        char_nb = ord(char)
                        if 64 < char_nb < 90:
                            message += char
                    # Le message est de type bytes.
                    # Le decode permet de transformer une valeur
                    # de type bytes en type str
                    decrypted = enigmalib.enigma(
                        bytes(message, 'utf-8')).decode("utf-8")
                    print(f"Nouveau message:", decrypted)
                    user_input = input(
                        "Press Enter to continue or "
                        "enter \"end\" here: ")
                time.sleep(1)


def recv(filename,
         right_shift=None,
         shift=None,
         enigma=False,
         statistics=False):
    """
    lit les nouveaux messages reçus dans un fichier
    :param filename: str : nom du fichier a lire
    :param right_shift: bool
    :param shift: int : nombre de decalages a appliquer sur chaque char
    """

    with open(filename) as f:
        if statistics:
            recv_caesar_microseconds = []
            recv_enigma_microseconds = []

            right_shift = True
            shift = 3
            with open(filename) as f:
                # On charge la librarie
                enigmalib = CDLL("./enigma.so")
                # On definit le type de retour de la fonction enigma. c_char_p indique
                # que le type de retour est char∗ (c_char_p = C char pointer)
                enigmalib.enigma.restype = c_char_p
                # On indique le type du seul argument, soit c_char_p. Notez que
                # nous assignons une liste, sachant qu’une fonction peut avoir
                # plusieurs arguments.
                enigmalib.enigma.argstype = [c_char_p]
                for i in range(100):
                    line = f.readline()
                    line = line.strip()
                    if line:
                        print("CAESAR")
                        caesar_start = time.time()
                        print(f"Nouveau message: {line} "
                              f"-> {uncaesar(line, right_shift, shift)}")
                        caesar_end = time.time()
                        execution_time = caesar_end - caesar_start
                        recv_caesar_microseconds.append(
                            int(execution_time * 1e6))

                    line = f.readline()
                    print(line)
                    line = line.strip()
                    if line:
                        print("ENIGMA")
                        message = ''
                        for i in range(len(line)):
                            char = line[i]
                            char_nb = ord(char)
                            if 64 < char_nb < 90:
                                message += char
                        enigma_start = time.time()

                        decrypted = enigmalib.enigma(
                            bytes(message, 'utf-8')).decode("utf-8")
                        enigma_end = time.time()
                        execution_time = enigma_end - enigma_start
                        recv_enigma_microseconds.append(
                            int(execution_time * 1e6))
                        print(f"Nouveau message:", decrypted)
            print()
            with open("execution-times.txt", "a") as e:
                e.write("recv_caesar_microseconds = " +
                        str(recv_caesar_microseconds))
                e.write("recv_enigma_microseconds = " +
                        str(recv_enigma_microseconds))
                e.close()


def main():
    if sys.argv[1] in ("G", "D"):
        if len(sys.argv) < 4:
            print("Utilisation: python recv.py G|D décalage fichier.",
                  file=sys.stderr)
            sys.exit(1)
        Asterix().decrypt()

    elif sys.argv[1] == "enigma":
        at = AlanTuring()
        at.filename = sys.argv[2]
        at.decrypt()

    elif sys.argv[1] == "statistics":
        filename: str = sys.argv[2]
        recv(filename=filename, statistics=True)

    else:
        print(
            f"Le premier argument doit être G ou D, {sys.argv[1]} est "
            f"incorrect",
            file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
