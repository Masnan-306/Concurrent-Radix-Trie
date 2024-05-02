import random
from collections import defaultdict

def read_words(file_path):
    """Reads words from a text file where each line contains one word."""
    with open(file_path, 'r') as file:
        words = file.read().splitlines()
    return words

def categorize_words(words):
    """Categorizes words into a dictionary by their starting letter."""
    categorized = defaultdict(list)
    for word in words:
        first_letter = word[0].lower()  # Consider case-insensitive
        categorized[first_letter].append(word)
    return categorized

def select_words(categorized_words, count=100):
    """Selects a specified number of words from each category."""
    selected_words = {}
    for letter, word_list in categorized_words.items():
        # Shuffle the word list to ensure randomness
        random.shuffle(word_list)
        # Take up to 'count' words from the shuffled list
        selected_words[letter] = word_list[:count]
    return selected_words

def save_to_txt(data, file_path):
    """Saves all selected words into a single text file."""
    with open(file_path, 'w') as file:
        for letter, words in data.items():
            for word in words:
                file.write(word + '\n')

def main():
    # Provide the path to your text file
    input_file = 'easy.txt'
    output_file = 'easy_selected_words.txt'

    # Step 1: Read words from the text file
    words = read_words(input_file)

    # Step 2: Categorize words by their starting letter
    categorized_words = categorize_words(words)

    # Step 3: Select up to 2000 words per category
    selected_words = select_words(categorized_words)

    # Step 4: Save all selected words into a single text file
    save_to_txt(selected_words, output_file)

if __name__ == "__main__":
    main()
