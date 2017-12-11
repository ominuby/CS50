import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # list constructed
        self.positive_words = []
        self.negative_words = []
        # load positive words
        with open(positives,'r') as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.positive_words.append(line.strip("\n"))
        # load negative words
        with open(negatives,'r') as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.negative_words.append(line.strip("\n"))


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # tokenize text by self.tokenizer,tokenize(text)
        tknzr = nltk.tokenize.TweetTokenizer()
        token = tknzr.tokenize(text)
        # initilize score
        score = 0
        for word in token:
            # iterate over tokens and check if word is positive or negative by str.lower()
            if word.lower() in self.positive_words:
                score = score + 1
            elif word.lower() in self.negative_words:
                score = score - 1
        return score
