#include <math.h>
#include <stdlib.h>
#include "image.h"
#include "matrix.h"

// Run an activation function on each element in a matrix,
// modifies the matrix in place
// matrix m: Input to activation function
// ACTIVATION a: function to run
void activate_matrix(matrix m, ACTIVATION a)
{
    int i, j;
    for(i = 0; i < m.rows; ++i){
        double sum = 0;
        for(j = 0; j < m.cols; ++j){
            double x = m.data[i][j];
            if(a == LOGISTIC){
                // TODO
                m.data[i][j] = 1 / (1 + exp(-1 * x));
            } else if (a == RELU){
                // TODO
                if (x <= 0) {
                  m.data[i][j] = 0;
                }
            } else if (a == LRELU){
                // TODO
                if (x <= 0) {
                  m.data[i][j] = 0.1 * x;
                }
            } else if (a == SOFTMAX){
                // TODO
                m.data[i][j] = exp(x);
            }
            sum += m.data[i][j];
        }
        if (a == SOFTMAX) {
            // TODO: have to normalize by sum if we are using SOFTMAX
            for (j = 0; j < m.cols; j++) {
                double val = m.data[i][j];
                m.data[i][j] = val / sum;
            }
        }
    }
}

// Calculates the gradient of an activation function and multiplies it into
// the delta for a layer
// matrix m: an activated layer output
// ACTIVATION a: activation function for a layer
// matrix d: delta before activation gradient
void gradient_matrix(matrix m, ACTIVATION a, matrix d)
{
    int i, j;
    for(i = 0; i < m.rows; ++i){
        for(j = 0; j < m.cols; ++j){
            double x = m.data[i][j];
            double delta = d.data[i][j];
            // TODO: multiply the correct element of d by the gradient
            if (a == LOGISTIC) {
              d.data[i][j] = delta * (x * (1 - x));
            } else if (a == RELU) {
              if (x <= 0) {
                d.data[i][j] = 0;
              }
            } else if (a == LRELU) {
              if (x <= 0) {
                d.data[i][j] = 0.1 * delta;
              }
            }
        }
    }
}

// Forward propagate information through a layer
// layer *l: pointer to the layer
// matrix in: input to layer
// returns: matrix that is output of the layer
matrix forward_layer(layer *l, matrix in)
{

    l->in = in;  // Save the input for backpropagation


    // TODO: fix this! multiply input by weights and apply activation function.
    matrix out = make_matrix(in.rows, l->w.cols);
    out = matrix_mult_matrix(in, l->w);
    activate_matrix(out, l->activation);

    free_matrix(l->out);// free the old output
    l->out = out;       // Save the current output for gradient calculation
    return out;
}

// Backward propagate derivatives through a layer
// layer *l: pointer to the layer
// matrix delta: partial derivative of loss w.r.t. output of layer
// returns: matrix, partial derivative of loss w.r.t. input to layer
matrix backward_layer(layer *l, matrix delta)
{
    // 1.4.1
    // delta is dL/dy
    // TODO: modify it in place to be dL/d(xw)
    gradient_matrix(l->out, l->activation, delta);


    // 1.4.2
    // TODO: then calculate dL/dw and save it in l->dw
    free_matrix(l->dw);
    matrix dw = matrix_mult_matrix(transpose_matrix(l->in), delta); // replace this
    l->dw = dw;


    // 1.4.3
    // TODO: finally, calculate dL/dx and return it.
    matrix dx = matrix_mult_matrix(delta, transpose_matrix(l->w)); // replace this

    return dx;
}

// Update the weights at layer l
// layer *l: pointer to the layer
// double rate: learning rate
// double momentum: amount of momentum to use
// double decay: value for weight decay
void update_layer(layer *l, double rate, double momentum, double decay)
{
    // TODO:
    // Calculate Δw_t = dL/dw_t - λw_t + mΔw_{t-1}
    // save it to l->v
    matrix wd = make_matrix(l->w.rows, l->w.cols);
    for (int i = 0; i < l->w.rows; i++) {
        for (int j = 0; j < l->w.cols; j++) {
            wd.data[i][j] = decay * l->w.data[i][j];
        }
    }

    matrix m = make_matrix(l->v.rows, l->v.cols);
    for (int i = 0; i < l->v.rows; i++) {
        for (int j = 0; j < l->v.cols; j++) {
            m.data[i][j] = momentum * l->v.data[i][j];
        }
    }

    l->v = axpy_matrix(1, l->dw, axpy_matrix(-1, wd, m));

    // Update l->w
    l->w = axpy_matrix(rate, l->v, l->w);


    // Remember to free any intermediate results to avoid memory leaks
}

// Make a new layer for our model
// int input: number of inputs to the layer
// int output: number of outputs from the layer
// ACTIVATION activation: the activation function to use
layer make_layer(int input, int output, ACTIVATION activation)
{
    layer l;
    l.in  = make_matrix(1,1);
    l.out = make_matrix(1,1);
    l.w   = random_matrix(input, output, sqrt(2./input));
    l.v   = make_matrix(input, output);
    l.dw  = make_matrix(input, output);
    l.activation = activation;
    return l;
}

// Run a model on input X
// model m: model to run
// matrix X: input to model
// returns: result matrix
matrix forward_model(model m, matrix X)
{
    int i;
    for(i = 0; i < m.n; ++i){
        X = forward_layer(m.layers + i, X);
    }
    return X;
}

// Run a model backward given gradient dL
// model m: model to run
// matrix dL: partial derivative of loss w.r.t. model output dL/dy
void backward_model(model m, matrix dL)
{
    matrix d = copy_matrix(dL);
    int i;
    for(i = m.n-1; i >= 0; --i){
        matrix prev = backward_layer(m.layers + i, d);
        free_matrix(d);
        d = prev;
    }
    free_matrix(d);
}

// Update the model weights
// model m: model to update
// double rate: learning rate
// double momentum: amount of momentum to use
// double decay: value for weight decay
void update_model(model m, double rate, double momentum, double decay)
{
    int i;
    for(i = 0; i < m.n; ++i){
        update_layer(m.layers + i, rate, momentum, decay);
    }
}

// Find the index of the maximum element in an array
// double *a: array
// int n: size of a, |a|
// returns: index of maximum element
int max_index(double *a, int n)
{
    if(n <= 0) return -1;
    int i;
    int max_i = 0;
    double max = a[0];
    for (i = 1; i < n; ++i) {
        if (a[i] > max){
            max = a[i];
            max_i = i;
        }
    }
    return max_i;
}

// Calculate the accuracy of a model on some data d
// model m: model to run
// data d: data to run on
// returns: accuracy, number correct / total
double accuracy_model(model m, data d)
{
    matrix p = forward_model(m, d.X);
    int i;
    int correct = 0;
    for(i = 0; i < d.y.rows; ++i){
        if(max_index(d.y.data[i], d.y.cols) == max_index(p.data[i], p.cols)) ++correct;
    }
    return (double)correct / d.y.rows;
}

// Calculate the cross-entropy loss for a set of predictions
// matrix y: the correct values
// matrix p: the predictions
// returns: average cross-entropy loss over data points, 1/n Σ(-ylog(p))
double cross_entropy_loss(matrix y, matrix p)
{
    int i, j;
    double sum = 0;
    for(i = 0; i < y.rows; ++i){
        for(j = 0; j < y.cols; ++j){
            sum += -y.data[i][j]*log(p.data[i][j]);
        }
    }
    return sum/y.rows;
}


// Train a model on a dataset using SGD
// model m: model to train
// data d: dataset to train on
// int batch: batch size for SGD
// int iters: number of iterations of SGD to run (i.e. how many batches)
// double rate: learning rate
// double momentum: momentum
// double decay: weight decay
void train_model(model m, data d, int batch, int iters, double rate, double momentum, double decay)
{
    int e;
    for(e = 0; e < iters; ++e){
        data b = random_batch(d, batch);
        matrix p = forward_model(m, b.X);
        fprintf(stderr, "%06d: Loss: %f\n", e, cross_entropy_loss(b.y, p));
        matrix dL = axpy_matrix(-1, p, b.y); // partial derivative of loss dL/dy
        backward_model(m, dL);
        update_model(m, rate/batch, momentum, decay);
        free_matrix(dL);
        free_data(b);
    }
}


// Questions
//
// 5.2.2.1 Why might we be interested in both training accuracy and testing accuracy? What do these two numbers tell us about our current model?
// The training accuracy tells us how well our model performs on the data it was trained wiht, and the testing accuracy tells us how well our model
// performs on the data that it has not seen during training. If the model performs good on the training data but bad on the testing data, then our
// model is too biased towards the training data. If the training accuracy is bad, then perhaps the model has not been trained properly at all. In
// general, the testing accuracy tells us how well the model will do in general on any new random data.

// 5.2.2.2 Try varying the model parameter for learning rate to different powers of 10 (i.e. 10^1, 10^0, 10^-1, 10^-2, 10^-3) and training the model. What patterns do you see and how does the choice of learning rate affect both the loss during training and the final model accuracy?
// As learning rate goes from 10 to 0.0001, loss during training decreases up till about 0.1-0.4
// (this is when the learning rate is 0.1) then increases again.
// The model accuracy increases as learning rate decreases until when the learning rate is 0.1.
// Here it reaches its peak--training accuracy is 0.92175 and test accuracy is 0.9178--and then the accuracy decreases as learning rate decreases more than 0.01.
// The model accuracy is worse when learning rate is 10, as compared to when it is about 0.0001.

// 5.2.2.3 Try varying the parameter for weight decay to different powers of 10: (10^0, 10^-1, 10^-2, 10^-3, 10^-4, 10^-5). How does weight decay affect the final model training and test accuracy?
// As the weight decay goes from 0.00001 to 1, the final model training and test accuracy gets worse.
// The accuracy is very similar as the weight decay goes from 0.00001 to 0.01, and then the accuracy gets worse as we increase weight decay.
// When decay is 1, training accuracy is 0.8961833333333333 and test accuracy is 0.8991, and when decay is 0.00001,
// training accuracy is 0.92175 and test accuracy is 0.9178.

// 5.2.3.1 Currently the model uses a logistic activation for the first layer. Try using a the different activation functions we programmed. How well do they perform? What's best?
// RELU and LRELU performed around the same with training accuracies of 0.92583 and 0.92342, respectively, and test accuracies of 0.9273 and 0.9251, respectively. LOGISTIC performed slightly worst than RELU and LRELU with a training accuracy of 0.88743 and test accuracy of 0.8929. SOFTMAX performed the worst of all the activation functions with a training accuracy of 0.5703 and test accuracy of 0.5792. The best activation function was RELU.

// 5.2.3.2 Using the same activation, find the best (power of 10) learning rate for your model. What is the training accuracy and testing accuracy?
// Using RELU for the activation function of the first layer, the best learning rate for the model is 0.1, and it gives us a training accuracy of 0.9653333333333334 and a testing accuracy of 0.96.

// 5.2.3.3 Right now the regularization parameter `decay` is set to 0. Try adding some decay to your model. What happens, does it help? Why or why not may this be?
// We used RELU activation function for the first layer and a learning rate of 0.1 while adding decay to the model.
// As the weight decay goes from 0.00001 to 0.1, the final model training and test accuracy stays about the same and gets worse when the weight decay is 1.
// The accuracy is very similar as the weight decay goes from 0.00001 to 0.1, and then the accuracy gets worse as we increase weight decay.
// When decay is 1, training accuracy is 0.9249333333333334 and test accuracy is 0.9287, and when decay is 0.00001,
// training accuracy is 0.9650833333333333 and test accuracy is 0.9603.
// Adding decay to the model does not help increase the training or the testing accuracy - both
// training and testing accuracy decrease as decay increases. But adding decay
// does help in reducing the difference between the test and training accuracy. As the decay increases,
// the difference between the test and training accuracy decreases, due to the model not overfitting
// the training data.

// 5.2.3.4 Modify your model so it has 3 layers instead of two. The layers should be `inputs -> 64`, `64 -> 32`, and `32 -> outputs`. Also modify your model to train for 3000 iterations instead of 1000. Look at the training and testing error for different values of decay (powers of 10, 10^-4 -> 10^0). Which is best? Why?
// We used RELU activation for the first 2 layers and a learning rate of 0.1.
// With the three layers as specified with a decay of 0.0001, we have the training accuracy as 0.9859166666666667 and the test accuracy as 0.9743.
// With a decay of 0.001 we have training accuracy as 0.98215 and test accuracy as 0.9701.
// With a decay of 0.01 we have the training accuracy as 0.98405 and test accuracy as 0.9741.
// With a decay of 0.1 we have the training accuracy as 0.9754666666666667 and test accuracy as 0.9693.
// Finally, with a decay of 1, we have the training accuracy as 0.92125 and test accuracy as 0.9191.
// The best test accuracy happens when the decay is 0.01, because this is the value for which the model is least likely
// to overfit, but still learn properly from the training data.

// 5.3.2.1 How well does your network perform on the CIFAR dataset?
// We got a training accuracy of 0.4543 and a test accuracy of 0.4399 with the 3 layer model from question 5.2.3.4. and the following values:
// batch = 128
// iters = 3000
// rate = 0.01
// momentum = .9
// decay = 0.1
