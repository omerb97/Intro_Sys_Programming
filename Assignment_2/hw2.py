

def printCompetitor(competitor):
    '''
    Given the data of a competitor, the function prints it in a specific format.
    Arguments:
        competitor: {'competition name': competition_name, 'competition type': competition_type,
                        'competitor id': competitor_id, 'competitor country': competitor_country, 
                        'result': result}
    '''
    competition_name = competitor['competition name']
    competition_type = competitor['competition type']
    competitor_id = competitor['competitor id']
    competitor_country = competitor['competitor country']
    result = competitor['result']
    
    assert(isinstance(result, int)) # Updated. Safety check for the type of result

    print(f'Competitor {competitor_id} from {competitor_country} participated in {competition_name} ({competition_type}) and scored {result}')


def printCompetitionResults(competition_name, winning_gold_country, winning_silver_country, winning_bronze_country):
    '''
    Given a competition name and its champs countries, the function prints the winning countries 
        in that competition in a specific format.
    Arguments:
        competition_name: the competition name
        winning_gold_country, winning_silver_country, winning_bronze_country: the champs countries
    '''
    undef_country = 'undef_country'
    countries = [country for country in [winning_gold_country, winning_silver_country, winning_bronze_country] if country != undef_country]
    print(f'The winning competitors in {competition_name} are from: {countries}')


def key_sort_competitor(competitor):
    '''
    A helper function that creates a special key for sorting competitors.
    Arguments:
        competitor: a dictionary contains the data of a competitor in the following format: 
                    {'competition name': competition_name, 'competition type': competition_type,
                        'competitor id': competitor_id, 'competitor country': competitor_country, 
                        'result': result}
    '''
    competition_name = competitor['competition name']
    result = competitor['result']
    return (competition_name, result)


def readParseData(file_name):
    '''
    Given a file name, the function returns a list of competitors.
    Arguments: 
        file_name: the input file name. Assume that the input file is in the directory of this script.
    Return value:
        A list of competitors, such that every record is a dictionary, in the following format:
            {'competition name': competition_name, 'competition type': competition_type,
                'competitor id': competitor_id, 'competitor country': competitor_country, 
                'result': result}
    '''
    with open(file_name, 'r') as f:
        lines = f.readlines()
    competitors_in_competitions = []
    competitors_list = []
    for line in lines:
        cut_word = line.split()
        if cut_word[0] == "competitor":
            temp_competitor_dict = {
                "competitor id": cut_word[1],
                "competitor country": cut_word[2]
            }
            competitors_list.append(temp_competitor_dict)
    for line in lines:
        cut_word = line.split()
        if cut_word[0] == "competition":
            competitor_id = cut_word[2]
            temp_competition_dict = {
                "competition name": cut_word[1],
                "competition type": cut_word[3],
                "competitor id": competitor_id,
                "competitor country": findCountry(competitor_id,competitors_list),
                "result": int(cut_word[4])
            }
            competitors_in_competitions.append(temp_competition_dict)
    return competitors_in_competitions

def disqualifyCheaters (one_sport):
    only_ID = [item["competitor id"] for item in one_sport]
    for i, athlete_ID in enumerate(only_ID):
        if only_ID.count(athlete_ID) > 1:
            #maybe need an if in the lamda, maybe not
            one_sport = list(filter(lambda x, ID=athlete_ID:(x["competitor id"] != ID), one_sport))
    return one_sport

def findCountry(competitor_id,competitors_list):
    for current_id in competitors_list:
        if current_id["competitor id"] == competitor_id:
            return current_id["competitor country"]

def calcCompetitionsResults(competitors_in_competitions):
    '''
    Given the data of the competitors, the function returns the champs countries for each competition.
    Arguments:
        competitors_in_competitions: A list that contains the data of the competitors
                                    (see readParseData return value for more info)
    Retuen value:
        A list of competitions and their champs (list of lists).
        Every record in the list contains the competition name and the champs, in the following format:
        [competition_name, winning_gold_country, winning_silver_country, winning_bronze_country]
    '''
    competitions_champs = []
    sport = list(set([val["competition name"] for val in competitors_in_competitions]))

    for branch in sport:
        final_element = [branch]
        individual_sport = [element for element in competitors_in_competitions if element["competition name"]==branch]
        individual_sport = disqualifyCheaters(individual_sport)
        if len(individual_sport) < 1:
            continue
        if individual_sport[0]["competition type"] == "untimed":
            individual_sport.sort(key= lambda e: e['result'], reverse=True)
        else:
            individual_sport.sort(key= lambda e: e['result'])
        top_three = individual_sport[:3]
        winners = []
        for element in top_three:
            winners.append(element["competitor country"])
        if len(winners) >= 1:
            while len(winners) < 3:
                winners.append("undef_country")
            final_element.extend(winners)
            competitions_champs.append(final_element)

    return competitions_champs


def partA(file_name = 'input.txt', allow_prints = True):
    # read and parse the input file
    competitors_in_competitions = readParseData(file_name)
    if allow_prints:
        # competitors_in_competitions are sorted by competition_name (string) and then by result (int)
        for competitor in sorted(competitors_in_competitions, key=key_sort_competitor):
            printCompetitor(competitor)
    
    # calculate competition results
    competitions_results = calcCompetitionsResults(competitors_in_competitions)
    if allow_prints:
        for competition_result_single in sorted(competitions_results):
            printCompetitionResults(*competition_result_single)
    
    return competitions_results


def partB(file_name = 'input.txt'):
    import Olympics
    competitions_results = partA(file_name, allow_prints = False)
    o = Olympics.OlympicsCreate()

    for competition in competitions_results:
        Olympics.OlympicsUpdateCompetitionResults(o, str(competition[1]), str(competition[2]), str(competition[3]))
    Olympics.OlympicsWinningCountry(o)
    Olympics.OlympicsDestroy(o)


if __name__ == "__main__":
    '''
    The main part of the script.
    __main__ is the name of the scope in which top-level code executes.
    
    To run only a single part, comment the line below which correspondes to the part you don't want to run.
    '''    
    file_name = 'input.txt'

    partA(file_name)
    partB(file_name)
