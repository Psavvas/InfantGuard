document.addEventListener('DOMContentLoaded', function() {
    // Handle phone number input to keep +1 prefix
    const phoneInput = document.getElementById('phoneNumber');
    if (phoneInput) {
        // Set cursor position after +1 when field is focused
        phoneInput.addEventListener('focus', function() {
            if (this.value === '+1') {
                setTimeout(() => {
                    this.setSelectionRange(2, 2);
                }, 0);
            }
        });

        // Prevent deletion of +1 prefix and remove spaces and special characters
        phoneInput.addEventListener('input', function() {
            // Remove all spaces and non-digit characters (except +1 prefix)
            let cleaned = this.value.replace(/[^\d+]/g, '');
            
            // Keep only digits after +1
            if (cleaned.startsWith('+1')) {
                cleaned = '+1' + cleaned.substring(2).replace(/\D/g, '');
            } else if (cleaned.startsWith('+')) {
                cleaned = '+1' + cleaned.substring(1).replace(/\D/g, '');
            } else {
                cleaned = '+1' + cleaned.replace(/\D/g, '');
            }
            
            this.value = cleaned;
            
            // Check phone number length
            if (cleaned.length > 0 && cleaned.length !== 12) {
                this.setCustomValidity('Please enter a valid 10-digit phone number. No spaces or special characters.');
            } else {
                this.setCustomValidity('');
            }
        });

        // Prevent user from deleting +1
        phoneInput.addEventListener('keydown', function(e) {
            const cursorPosition = this.selectionStart;
            if ((e.key === 'Backspace' && cursorPosition <= 2) || 
                (e.key === 'Delete' && cursorPosition < 2)) {
                e.preventDefault();
            }
        });
    }

    const form = document.getElementById('setupForm');
    
    if (form) {
        form.addEventListener('submit', function(e) {
            e.preventDefault();

            // Collect form data
            const formData = {
                deviceId: document.getElementById('deviceId').value,
                phoneNumber: document.getElementById('phoneNumber').value,
                licensePlate: document.getElementById('licensePlate').value,
                vehicleMake: document.getElementById('vehicleMake').value,
                vehicleModel: document.getElementById('vehicleModel').value,
                vehicleColor: document.getElementById('vehicleColor').value,
            };

            localStorage.setItem('infantGuardConfig', JSON.stringify(formData));

            // Send data to Cloudflare database
            sendToDatabase(formData);
        });
    }
});

async function sendToDatabase(formData) {
    try {
        const WORKER_URL = 'https://infantguard-activation.pauldsavvas.workers.dev';
        
        console.log('Sending data to worker:', WORKER_URL);
        console.log('Form data:', JSON.stringify(formData, null, 2));
        
        const response = await fetch(WORKER_URL, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(formData)
        });

        console.log('Response status:', response.status);
        const data = await response.json();
        console.log('Response data:', JSON.stringify(data, null, 2));
        
        if (data.success) {
            console.log('Device registered successfully');
            redirectToConfirmation();
        } else {
            console.error('Error registering device:', data.error || data);
            alert('Error registering the device. It may have already been registered. If the error persists, please contact support at: infantguard@paulsavvas.me.');
        }
    } catch (error) {
        console.error('Error sending data:', error);
        alert('Network error. Please check your connection.');
    }
}

function redirectToConfirmation() {
    // Reset form
    const form = document.getElementById('setupForm');
    if (form) form.reset();

    // Redirect to confirmation page
    setTimeout(() => {
        window.location.href = 'confirmation.html';
    }, 250);
}
