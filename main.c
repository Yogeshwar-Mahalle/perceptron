//
//  main.c
//  perceptron
//
//  Created by Yogeshwar Mahalle on 23/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define MAX_INPUT       100
#define MAX_OUTPUT      100
#define MAX_ITERATION   5000
#define EPSILON         1e-7
#define LEARNING_RATE   1e-3

double randomFloat(void)
{
    return (double)rand() / (double)RAND_MAX;
}

//Activation function for digital decisions
int sigmoid(double x)
{
    return 1.0f/(1.0f + exp(-x));
}


double calculatePerceptronAccuracy(int dataCount, double x1[], double x2[], double output[], double weights[], double bias[], int bDigitalTraining)
{
    double result = 0.0f;
    for (int i = 0; i < dataCount; i++)
    {
        double y = 0;
        if( bDigitalTraining == 0 )
            y = weights[0] * x1[i] + weights[1] * x2[i] + bias[0];
        else
            y = sigmoid(weights[0] * x1[i] + weights[1] * x2[i] + bias[0]);
        
        double err = y - output[i];
        
        result += err * err; //square to make number positive for addition
    }
    return result;
}

void trainPerceptron(int trainingDataCount, double inputs1[], double inputs2[], double outputs[], double weights[], double bias[], int bDigitalTraining)
{
    double totalError;
    int iteration = 0;
    do {

        //Finite Difference for back propagation
        totalError = calculatePerceptronAccuracy(trainingDataCount, inputs1, inputs2, outputs, weights, bias, bDigitalTraining);
        double weightsDiff[2];
        weightsDiff[0] = weights[0] + EPSILON;
        weightsDiff[1] = weights[1] ;
        double totalErrorWeight1Diff = calculatePerceptronAccuracy(trainingDataCount, inputs1, inputs2, outputs, weightsDiff, bias, bDigitalTraining);
        double finiteWeight1Diff =  ( totalErrorWeight1Diff - totalError ) / EPSILON;
        
        weightsDiff[0] = weights[0] ;
        weightsDiff[1] = weights[1] + EPSILON;
        double totalErrorWeight2Diff = calculatePerceptronAccuracy(trainingDataCount, inputs1, inputs2, outputs, weightsDiff, bias, bDigitalTraining);
        double finiteWeight2Diff =  ( totalErrorWeight2Diff - totalError ) / EPSILON;
        
        double biasDiff[1];
        biasDiff[0] = bias[0] + EPSILON;
        double totalErrorBiasDiff = calculatePerceptronAccuracy(trainingDataCount, inputs1, inputs2, outputs, weights, biasDiff, bDigitalTraining);
        double finiteBiasDiff =  ( totalErrorBiasDiff - totalError ) / EPSILON;
        
        //Small correction is weight and biase to reduce error
        weights[0] -= LEARNING_RATE * finiteWeight1Diff;
        weights[1] -= LEARNING_RATE * finiteWeight2Diff;
        bias[0] -= LEARNING_RATE * finiteBiasDiff;
        
        totalError = calculatePerceptronAccuracy(trainingDataCount, inputs1, inputs2, outputs, weights, bias, bDigitalTraining);

        printf("Iteration %d : ERROR = %.9f : WEIGHT1 = %.9f : WEIGHT1 = %.9f : BIAS : %.9f \n", iteration, totalError, weights[0], weights[1], bias[0]);
        
        iteration++;
        
    } while ( (totalError * 10000000) > 0.00001f && iteration <= MAX_ITERATION);
    
}


int main(int argc, const char * argv[])
{
    char anlogVsDigital = 'A';
    int bDigitalTraining = 0;
    double inputs1[MAX_INPUT], inputs2[MAX_INPUT], outputs[MAX_OUTPUT], weights[2], bias[1], userInput1, userInput2;
    int trainingDataCount, iteration;
    
    printf("Is the perceptron trained for digital data or analog data? (A/D) : ");
    scanf("%c", &anlogVsDigital);
    bDigitalTraining = bDigitalTraining == 'D' || bDigitalTraining == 'd' ? 1 : 0;
    
    //srand( (unsigned)time(NULL) );
    srand( 69 );
    weights[0] = randomFloat();
    weights[1] = randomFloat();
    bias[0] = randomFloat();

    FILE *fp;
    if ((fp = fopen("test.txt", "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }

    trainingDataCount = 0;
    while (fscanf(fp, "%lf %lf %lf", &inputs1[trainingDataCount], &inputs2[trainingDataCount], &outputs[trainingDataCount]) != EOF) {
        trainingDataCount++;
    }

    //train perceptron model
    trainPerceptron( trainingDataCount, inputs1, inputs2, outputs, weights, bias, bDigitalTraining);

    printf("\nDecision boundary (line) equation: y = %.9f * x1 + %.9f * x2 + %.9f \n", weights[0], weights[1], bias[0]);
    
    printf("Enter your input data to get result from trained perceptron model : ");
    scanf("%lf %lf", &userInput1, &userInput2);
    
    double result;
    if(bDigitalTraining)
        result = sigmoid(weights[0] * userInput1 + weights[1] * userInput2 + bias[0]);
    else
        result = (weights[0] * userInput1 + weights[1] * userInput2 + bias[0]);
    
    printf("The result for your input data %f %f from trained perceptron model is %f\n", userInput1, userInput2, result);
    

    return 0;
    
    
}
